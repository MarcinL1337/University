#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define DEBUG 0
#include "shell.h"

sigset_t sigchld_mask;

static void sigint_handler(int sig) {
  /* No-op handler, we just need break read() call with EINTR. */
  (void)sig;
}

/* Rewrite closed file descriptors to -1,
 * to make sure we don't attempt do close them twice. */
static void MaybeClose(int *fdp) {
  if (*fdp < 0)
    return;
  Close(*fdp);
  *fdp = -1;
}

/* Consume all tokens related to redirection operators.
 * Put opened file descriptors into inputp & output respectively. */
static int do_redir(token_t *token, int ntokens, int *inputp, int *outputp) {
  token_t mode = NULL; /* T_INPUT, T_OUTPUT or NULL */
  int n = 0;           /* number of tokens after redirections are removed */

  for (int i = 0; i < ntokens; i++) {
    /* TODO: Handle tokens and open files as requested. */
#ifdef STUDENT
    /*
    Jeżeli trafiamy na T_OUTPUT/T_INPUT, ustawiam token[i] na T_NULL oraz
    wywołuję Open z odpowiednimi uprawnieniami na token[i+1]. Gdy trafiamy na
    token[i] != T_NULL, inkrementuję n.
    */
    if (token[i] == T_OUTPUT) {
      MaybeClose(outputp);
      *outputp = Open(token[i + 1], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
      token[i] = T_NULL;
      token[i + 1] = T_NULL;
    } else if (token[i] == T_INPUT) {
      MaybeClose(inputp);
      *inputp = Open(token[i + 1], O_RDONLY, S_IRWXU);
      token[i] = T_NULL;
      token[i + 1] = T_NULL;
    } else if (token[i] != T_NULL)
      n++;
    (void)mode;
#endif /* !STUDENT */
  }

  token[n] = NULL;
  return n;
}

/* Execute internal command within shell's process or execute external command
 * in a subprocess. External command can be run in the background. */
static int do_job(token_t *token, int ntokens, bool bg) {
  int input = -1, output = -1;
  int exitcode = 0;

  ntokens = do_redir(token, ntokens, &input, &output);

  if (!bg) {
    if ((exitcode = builtin_command(token)) >= 0)
      return exitcode;
  }

  sigset_t mask;
  Sigprocmask(SIG_BLOCK, &sigchld_mask, &mask);

  /* TODO: Start a subprocess, create a job and monitor it. */
#ifdef STUDENT
  pid_t child_pid = Fork();
  /*
  W rodzicu ustawiam odpowiedni pgid, tworzę nowy job oraz dodaję proces do
  joba. Jeżeli bg == false, ustawiam proces na fg i ustawiam odpowiednio
  exitcode. Jezeli bg == true, printuję informację o wykonywaniu joba w tle.
  */
  if (child_pid) // parent
  {
    setpgid(child_pid, child_pid);
    int j = addjob(child_pid, bg);
    addproc(j, child_pid, token);
    if (!bg) {
      setfgpgrp(child_pid);
      exitcode = monitorjob(&mask);
    } else
      printf("[%d] running '%s'\n", j, jobcmd(j));

    MaybeClose(&input);
    MaybeClose(&output);
    /*
    W dziecku analogicznie ustawiam pgid, sprawdzam flagę bg i ustawiam proces
    na fg w zależności od jej wartości. Następnie przywracam domyślną obsługę
    wymaganych sygnałów, które w main zostały ustawione na SIG_IGN. Na koniec
    wywołuję external_command.
    */
  } else // child
  {
    Setpgid(0, 0);
    if (!bg)
      setfgpgrp(getpid());

    Signal(SIGTSTP, SIG_DFL);
    Signal(SIGTTIN, SIG_DFL);
    Signal(SIGTTOU, SIG_DFL);

    Sigprocmask(SIG_SETMASK, &mask, NULL);

    if (input != -1) {
      Dup2(input, STDIN_FILENO);
      Close(input);
    }

    if (output != -1) {
      Dup2(output, STDOUT_FILENO);
      Close(output);
    }
    external_command(token);
  }
#endif /* !STUDENT */

  Sigprocmask(SIG_SETMASK, &mask, NULL);
  return exitcode;
}

/* Start internal or external command in a subprocess that belongs to pipeline.
 * All subprocesses in pipeline must belong to the same process group. */
static pid_t do_stage(pid_t pgid, sigset_t *mask, int input, int output,
                      token_t *token, int ntokens, bool bg) {
  ntokens = do_redir(token, ntokens, &input, &output);

  if (ntokens == 0)
    app_error("ERROR: Command line is not well formed!");

  /* TODO: Start a subprocess and make sure it's moved to a process group. */
  pid_t pid = Fork();
#ifdef STUDENT
  /*
  Analogicznie jak w dziecku w do_job()
  */
  if (!pid) // child
  {
    setpgid(0, pgid);
    if (!bg)
      setfgpgrp(getpid());

    Signal(SIGTSTP, SIG_DFL);
    Signal(SIGTTIN, SIG_DFL);
    Signal(SIGTTOU, SIG_DFL);

    Sigprocmask(SIG_SETMASK, mask, NULL);

    if (input != -1) {
      Dup2(input, STDIN_FILENO);
      Close(input);
    }

    if (output != -1) {
      Dup2(output, STDOUT_FILENO);
      Close(output);
    }
    external_command(token);
  }
#endif /* !STUDENT */

  return pid;
}

static void mkpipe(int *readp, int *writep) {
  int fds[2];
  Pipe(fds);
  fcntl(fds[0], F_SETFD, FD_CLOEXEC);
  fcntl(fds[1], F_SETFD, FD_CLOEXEC);
  *readp = fds[0];
  *writep = fds[1];
}

/* Pipeline execution creates a multiprocess job. Both internal and external
 * commands are executed in subprocesses. */
static int do_pipeline(token_t *token, int ntokens, bool bg) {
  pid_t pid, pgid = 0;
  int job = -1;
  int exitcode = 0;

  int input = -1, output = -1, next_input = -1;

  mkpipe(&next_input, &output);

  sigset_t mask;
  Sigprocmask(SIG_BLOCK, &sigchld_mask, &mask);

  /* TODO: Start pipeline subprocesses, create a job and monitor it.
   * Remember to close unused pipe ends! */
#ifdef STUDENT
  /*
  Pierwszy proces z potoku rozważam osobno, aby odpowiednio ustawić pgid.
  Mallocuję pamieć na token_t *cur_process, który będzie przechowywał aktualny
  proces z potoku, a następnie wrzucam do niego po kolei tokeny aż do
  natrafienia na pierwszy T_PIPE. Tworzę nowego joba i dodaję pierwszy proces.
  */
  token_t *cur_process = malloc(ntokens * sizeof(token_t));
  int i;

  for (i = 0; i < ntokens; i++) {
    if (token[i] == T_PIPE)
      break;
    cur_process[i] = token[i];
  }

  cur_process[i] = T_NULL;
  i++;

  pgid = do_stage(0, &mask, input, output, cur_process, i, bg);
  job = addjob(pgid, bg);
  addproc(job, pgid, cur_process);
  MaybeClose(&input);
  MaybeClose(&output);

  /*
  W następnej pętli rozważam pozostałe procesy z potoku.
  Dopóki nie natrafię na T_PIPE lub T_NULL ustawiam tokeny aktualnie rozważanego
  procesu na token[k]. Gdy natrafiam na T_PIPE lub T_NULL, jeżeli nie jestem na
  ostatnim procesie (k+1 < ntokens), tworzę nową rurę, pid ustawiam na wartość z
  do_stage i ustawiam pgid każdego procesu na pid pierwszego procesu z potoku.
  Następnie dodaję proces do joba, zamykam nieużywane końce rury i zeruję
  indeks, którego używam do ustawiania tokenów w cur_process.
  */

  int j = 0;
  for (int k = i; k <= ntokens; k++) {
    if (token[k] == T_PIPE || token[k] == T_NULL) {
      input = next_input;
      cur_process[j] = T_NULL;

      if (k + 1 < ntokens)
        mkpipe(&next_input, &output);

      pid = do_stage(pgid, &mask, input, output, cur_process, j, bg);
      setpgid(pid, pgid);
      addproc(job, pid, cur_process);
      MaybeClose(&input);
      MaybeClose(&output);
      j = 0;
    } else {
      cur_process[j] = token[k];
      j++;
    }
  }

  /*
  Jeżeli bg == false, ustawiam procesy z grupy pgid na fg i ustawiam exitcode,
  wpp printuję informację o wystartowaniu potoku w tle.
  Na koniec zwalniam zmallocowaną pamięć na aktualny proces w potoku.
  */

  if (!bg) {
    setfgpgrp(pgid);
    exitcode = monitorjob(&mask);
  } else
    printf("[%d] running '%s'\n", job, jobcmd(job));

  free(cur_process);
#endif /* !STUDENT */

  Sigprocmask(SIG_SETMASK, &mask, NULL);
  return exitcode;
}

static bool is_pipeline(token_t *token, int ntokens) {
  for (int i = 0; i < ntokens; i++)
    if (token[i] == T_PIPE)
      return true;
  return false;
}

static void eval(char *cmdline) {
  bool bg = false;
  int ntokens;
  token_t *token = tokenize(cmdline, &ntokens);

  if (ntokens > 0 && token[ntokens - 1] == T_BGJOB) {
    token[--ntokens] = NULL;
    bg = true;
  }

  if (ntokens > 0) {
    if (is_pipeline(token, ntokens)) {
      do_pipeline(token, ntokens, bg);
    } else {
      do_job(token, ntokens, bg);
    }
  }

  free(token);
}

#ifndef READLINE
static char *readline(const char *prompt) {
  static char line[MAXLINE]; /* `readline` is clearly not reentrant! */

  write(STDOUT_FILENO, prompt, strlen(prompt));

  line[0] = '\0';

  ssize_t nread = read(STDIN_FILENO, line, MAXLINE);
  if (nread < 0) {
    if (errno != EINTR)
      unix_error("Read error");
    msg("\n");
  } else if (nread == 0) {
    return NULL; /* EOF */
  } else {
    if (line[nread - 1] == '\n')
      line[nread - 1] = '\0';
  }

  return strdup(line);
}
#endif

int main(int argc, char *argv[]) {
  /* `stdin` should be attached to terminal running in canonical mode */
  if (!isatty(STDIN_FILENO))
    app_error("ERROR: Shell can run only in interactive mode!");

#ifdef READLINE
  rl_initialize();
#endif

  sigemptyset(&sigchld_mask);
  sigaddset(&sigchld_mask, SIGCHLD);

  if (getsid(0) != getpgid(0))
    Setpgid(0, 0);

  initjobs();

  struct sigaction act = {
    .sa_handler = sigint_handler,
    .sa_flags = 0, /* without SA_RESTART read() will return EINTR */
  };
  Sigaction(SIGINT, &act, NULL);

  Signal(SIGTSTP, SIG_IGN);
  Signal(SIGTTIN, SIG_IGN);
  Signal(SIGTTOU, SIG_IGN);

  while (true) {
    char *line = readline("# ");

    if (line == NULL)
      break;

    if (strlen(line)) {
#ifdef READLINE
      add_history(line);
#endif
      eval(line);
    }
    free(line);
    watchjobs(FINISHED);
  }

  msg("\n");
  shutdownjobs();

  return 0;
}
