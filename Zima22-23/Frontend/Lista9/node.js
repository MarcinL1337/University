"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var Endpoints;
(function (Endpoints) {
    Endpoints["ELIXIRS"] = "Elixirs";
    Endpoints["SPELLS"] = "Spells";
    Endpoints["HOUSES"] = "Houses";
    Endpoints["INGREDIENTS"] = "Ingredients";
    Endpoints["WIZARDS"] = "Wizards";
})(Endpoints || (Endpoints = {}));
;
var Objects;
(function (Objects) {
    Objects[Objects["ELIXIRS"] = 0] = "ELIXIRS";
    Objects[Objects["SPELLS"] = 1] = "SPELLS";
    Objects[Objects["HOUSES"] = 2] = "HOUSES";
    Objects[Objects["INGREDIENTS"] = 3] = "INGREDIENTS";
    Objects[Objects["WIZARDS"] = 4] = "WIZARDS";
})(Objects || (Objects = {}));
;
const elixirs_diff = ['Unknown', 'Advanced', 'Moderate', 'Beginner', 'OrdinaryWizardingLevel', 'OneOfAKind'];
const spells_type = ['None', 'Charm', 'Conjuration', 'Spell', 'Transfiguration', 'HealingSpell', 'DarkCharm', 'Jinx', 'Curse', 'MagicalTransportation', 'Hex', 'CounterSpell', 'DarkArts', 'CounterJinx', 'CounterCharm', 'Untransfiguration', 'BindingMagicalContract', 'Vanishment'];
const spells_light = ['None', 'Blue', 'IcyBlue', 'Red', 'Gold', 'Purple', 'Transparent', 'White', 'Green', 'Orange', 'Yellow', 'BrightBlue', 'Pink', 'Violet', 'BlueishWhite', 'Silver', 'Scarlet', 'Fire', 'FieryScarlet', 'Grey', 'DarkRed', 'Turquoise', 'PsychedelicTransparentWave', 'BrightYellow', 'BlackSmoke'];
let elixirs;
let spells;
class FetchError extends Error {
    constructor(response) {
        if (typeof response === "number") {
            super(`Error code: ${response}`);
        }
        else {
            super(response);
        }
    }
}
const fetchData = (endpoint) => __awaiter(void 0, void 0, void 0, function* () {
    try {
        const response = yield fetch(`https://wizard-world-api.herokuapp.com/${endpoint}`, {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
            },
        });
        if (response.ok === false) {
            throw new FetchError(response.status);
        }
        return response.json();
    }
    catch (e) {
        if (e instanceof TypeError && e.message.includes("NetworkError")) {
            throw new FetchError(e.message);
        }
        throw e;
    }
});
const getElixirs = () => __awaiter(void 0, void 0, void 0, function* () {
    const data = yield fetchData(Endpoints.ELIXIRS);
    return data.filter(({ name, effect }) => (name === null || name === void 0 ? void 0 : name.length) > 0 && (effect === null || effect === void 0 ? void 0 : effect.length) > 0);
});
const getSpells = () => __awaiter(void 0, void 0, void 0, function* () {
    const data = yield fetchData(Endpoints.SPELLS);
    return data.filter(({ name, incantation }) => (name === null || name === void 0 ? void 0 : name.length) > 0 && (incantation === null || incantation === void 0 ? void 0 : incantation.length) > 0);
});
const getRandomNumber = (maxNumber) => Math.floor(Math.random() * maxNumber);
const getThreeOptions = (objects) => {
    const set = new Set();
    while (set.size < 3) {
        set.add(getRandomNumber(objects.length));
    }
    const [first, second, third] = set;
    return {
        option1: objects[first],
        option2: objects[second],
        option3: objects[third],
    };
};
const generateGame = (options, questionFunction) => {
    const valid = getRandomNumber(3);
    console.log(`Cheatmode: Valid option is option ${valid + 1}`);
    document.getElementById("question").innerText = questionFunction(options[valid].question);
    document.getElementById("option1").innerText = options[0].answer;
    document.getElementById("option2").innerText = options[1].answer;
    document.getElementById("option3").innerText = options[2].answer;
    document.getElementById("options").addEventListener("click", (e) => {
        const target = e.target; //cast e.target to HTMLElement so that it has required properties
        if (target.tagName !== "BUTTON")
            return;
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
        const options = [option1, option2, option3].map((option) => ({
            question: option.name,
            answer: option.effect,
        }));
        generateGame(options, (question) => `Elixir ${question} has effect:`);
    }
    if (game === Objects.SPELLS) {
        const { option1, option2, option3 } = getThreeOptions(spells);
        const options = [option1, option2, option3].map((option) => ({
            question: option.name,
            answer: option.incantation,
        }));
        generateGame(options, (question) => `Spell ${question} has incantation:`);
    }
};
const game = () => __awaiter(void 0, void 0, void 0, function* () {
    try {
        [elixirs, spells] = yield Promise.all([getElixirs(), getSpells()]);
        round();
    }
    catch (e) {
        document.getElementById("game").innerHTML = "";
        document.getElementById("question").innerHTML = "";
        if (e instanceof FetchError) {
            document.getElementById("response").innerText =
                "Problem with the connection. Try refreshing the page.";
            return;
        }
        document.getElementById("response").innerText = e.message;
    }
});
function isSpell(obj) {
    return obj.creator !== undefined;
}
function isElixir(obj) {
    return obj.manufacturer !== undefined;
}
game();
const check = (object) => {
    if (isSpell(object)) {
        console.log(object.incantation);
    }
    if (isElixir(object)) {
        console.log(object.ingredients);
    }
};
