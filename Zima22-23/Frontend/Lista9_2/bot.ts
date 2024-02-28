enum Endpoints {
  ELIXIRS = "Elixirs",
  SPELLS = "Spells",
  HOUSES = "Houses",
  INGREDIENTS = "Ingredients",
  WIZARDS = "Wizards",
}

enum Objects {
  ELIXIRS = 0,
  SPELLS = 1,
  HOUSES = 2,
  INGREDIENTS = 3,
  WIZARDS = 4,
}

interface Elixir {
  name: string;
  effect: string;
}

interface Spell {
  name: string;
  incantation: string;
}

interface House {
  name: string;
  headOfHouse: string;
  founder: string;
  ghost: string;
  values: string[];
  colors: string[];
}

interface Ingredient {
  name: string;
  type: string;
  effects: string[];
}

interface Wizard {
  name: string;
  house: string;
  patronus: string;
}

class FetchError extends Error {
  constructor(response: number | string) {
    if (typeof response === "number") {
      super(`Error code: ${response}`);
    } else {
      super(response);
    }
  }
}

const fetchData = async (endpoint: Endpoints): Promise<Elixir[] | Spell[] | House[] | Ingredient[] | Wizard[]> => {
  try {
    const response = await fetch(
      `https://wizard-world-api.herokuapp.com/${endpoint}`,
      {
        method: "GET",
        headers: {
          "Content-Type": "application/json",
        },
      }
    );

    if (response.ok === false) {
      throw new FetchError(response.status);
    }

    return response.json();
  } catch (e) {
    if (e instanceof TypeError && e.message.includes("NetworkError")) {
      throw new FetchError(e.message);
    }

    throw e;
  }
};

const getElixirs = async (): Promise<Elixir[]> => {
  const data = await fetchData(Endpoints.ELIXIRS);

  return data.filter(
    ({ name, effect }) => name?.length > 0 && effect?.length > 0
  );
};

const getSpells = async (): Promise<Spell[]> => {
  const data = await fetchData(Endpoints.SPELLS);

  return data.filter(
    ({ name, incantation }) => name?.length > 0 && incantation?.length > 0
  );
};

const getRandomNumber = (maxNumber: number): number => Math.floor(Math.random() * maxNumber);

interface GameOption {
  question: string;
  answer: string;
}

const getThreeOptions = (objects: GameOption[]): { option1: GameOption; option2: GameOption; option3: GameOption } => {
    const set = new Set<number>();

    while (set.size < 3) {
      set.add(getRandomNumber(objects.length));
    }
    
    const [first, second, third] = [...set];
    
    return {
      option1: objects[first],
      option2: objects[second],
      option3: objects[third],
    };
    };
    
    type QuestionFunction = (question: string) => string;
    
    const generateGame = (options: GameOption[], questionFunction: QuestionFunction) => {
    const valid = getRandomNumber(3);
    
    console.log(`Cheatmode: Valid option is option ${valid + 1}`);
    
    document.getElementById("question").innerText = questionFunction(
      options[valid].question
    );
    
    document.getElementById("option1").innerText = options[0].answer;
    document.getElementById("option2").innerText = options[1].answer;
    document.getElementById("option3").innerText = options[2].answer;
    
    document.getElementById("options").addEventListener("click", (e) => {
      const target = e.target;
    
      if (target.tagName !== "BUTTON") return;
    
      if (Number(target.dataset.option) === valid) {
        document.getElementById("response").innerText = "Good!";
        round();
        return;
      }
    
      document.getElementById("response").innerText = "Wrong!";
    });
    };
    
    const round = () => {
    const game = getRandomNumber(2);
    
    const optionsElement = `
      <div id="options">
        <button id="option1" data-option="0"></button>
        <button id="option2" data-option="1"></button>
        <button id="option3" data-option="2"></button>
      </div>
    `;
    
    document.getElementById("game").innerHTML = optionsElement;
    
    if (game === Objects.ELIXIRS) {
      const { option1, option2, option3 } = getThreeOptions(elixirs);
    
      generateGame(
        [option1, option2, option3],
        (question) => `Which elixir has the effect: ${question}`
      );
    } else {
      const { option1, option2, option3 } = getThreeOptions(spells);
    
      generateGame(
        [option1, option2, option3],
        (question) => `Which spell has the incantation: ${question}`
      );
    }
    };
    
    const startGame = async () => {
    elixirs = await getElixirs();
    spells = await getSpells();
    
    round();
    };
    
    startGame();
    
