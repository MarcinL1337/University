const pokedex = {};

window.onload = async function()
{
    for(let i = 1; i < 152; i++)
    {
        await getPokemon(i);

        let pokemon = document.createElement("div");
        pokemon.id = i;
        pokemon.innerText = i.toString() + ". " + pokedex[i]["name"].toUpperCase();
        pokemon.classList.add("pokemon_name");
        document.querySelector(".pokemon_list").append(pokemon);
        pokemon.addEventListener("click", displayInfo);
    }

    document.querySelector(".pokemon_description").innerText = pokedex[1]["description"];
}

async function getPokemon(num)
{
    let url = "https://pokeapi.co/api/v2/pokemon/" + num.toString();

    let response = await fetch(url);
    let pokemon = await response.json();

    let pokemonName = pokemon["name"];
    let pokemonType = pokemon["types"];
    let pokemonImg = pokemon["sprites"]["front_default"];

    res = await fetch(pokemon["species"]["url"]);
    let pokemonDesc = await res.json();

    pokemonDesc = pokemonDesc["flavor_text_entries"][6]["flavor_text"];

    pokedex[num] = {
        "name"        : pokemonName,
        "img"         : pokemonImg,
        "types"       : pokemonType,
        "description" : pokemonDesc
    }
}

function displayInfo()
{
    document.querySelector(".pokemon_img").src = pokedex[this.id]["img"];

    let pokemonTypes = document.querySelector(".pokemon_types");

    while(pokemonTypes.firstChild)
    {
        pokemonTypes.firstChild.remove();
    }

    let types = pokedex[this.id]["types"];
    for(let i = 0; i < types.length; i++)
    {
        let type = document.createElement("span");
        type.innerText = types[i]["type"]["name"].toUpperCase();
        type.classList.add("type");
        type.classList.add(types[i]["type"]["name"]);
        pokemonTypes.append(type);
    }

    document.querySelector(".pokemon_description").innerText = pokedex[this.id]["description"];
}