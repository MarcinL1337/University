const ul = document.querySelector('ul');
// const li = document.createElement('li');
const add_button = document.querySelector('.add_elem_button');
const del_button = document.querySelector('.del_elem_button');
const input = document.querySelector('.todo_input');

add_button.addEventListener("click", function addElem(e)
{
    e.preventDefault();
    if(input.value.length > 0) 
    {
        // const li = document.createElement('li');
        // li.innerText = input.value;
        // ul.append(li);

        ul.innerHTML +=
        `<li class="active_elem">${input.value} <button type="submit" class="del_elem_button">X</button> </li>`;

        input.value = "";
    }
    else
        alert("Element listy nie może być pusty!");
});

del_button.addEventListener("click", function delElem(e)
{

});

// const todoForm = document.querySelector(".todo_form");
// const todoInput = document.querySelector(".todo_input");
// const todoUl = document.querySelector(".list_of_elems");

// todoForm.addEventListener("submit", function(e)
// {
//     const input = todoInput.value;

//     if(!input)
//     {
//         alert("Nie możesz dodać pustego elementu!");
//     }

//     const li = document.createElement("li");
//     li.classList.add("active");

    
// })