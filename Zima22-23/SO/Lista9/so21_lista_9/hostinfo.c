#include "csapp.h"

int main(int argc, char **argv) {
  struct addrinfo *p, *listp, hints;
  char buf[MAXLINE];
  int rc, flags;
  struct servent *service = NULL;

  if (argc > 3)
    app_error("usage: %s <domain name> | <protocol name optional> |\n", argv[0]);

  if(argc == 3)
    service = getservbyname(argv[2], NULL);

  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM; // tutaj trzeba na 0 jezeli chcemy tftp
  /* Connections only */
  if ((rc = getaddrinfo(argv[1], service ? service->s_name : NULL, &hints, &listp)) != 0)
    gai_error(rc, "getaddrinfo");

  /* Walk the list and display each IP address */
  flags = NI_NUMERICHOST; /* Display address string instead of domain name */
  for (p = listp; p; p = p->ai_next) {
    Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);

    if (service && p->ai_family == AF_INET6)
      printf("[%s]:%d\n", buf, htons(service->s_port));
    else if (service)
      printf("%s:%d\n", buf, htons(service->s_port));
    else
      printf("%s\n", buf);

  }

  /* Clean up */
  freeaddrinfo(listp);

  return EXIT_SUCCESS;
}