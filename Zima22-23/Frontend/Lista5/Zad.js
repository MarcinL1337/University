let productList = [];

class Product 
{
    /**
     * @constructor
     * @param {String} id 
     * @param {String} name 
     * @param {String} quantity 
     * @param {String} date_to_buy 
     * @param {String} status 
     * @param {String} cost_per_unit 
     */
    constructor(id, name, quantity, date_to_buy, status, cost_per_unit)
    {
        this.id = Number(id);
        this.name = name;
        this.quantity = Number(quantity);
        this.date_to_buy = date_to_buy;
        if (status == "False") this.status = "Not bought";
        else this.status = "Bought";
        if(cost_per_unit !== undefined)
            this.cost_per_unit = Number(cost_per_unit);
        else this.cost_per_unit = -1;
    }

    /**
     * Pretty printing product's info
     */
    printProd()
    {
        if(isNaN(this.cost_per_unit))
            console.log("ID: " + this.id, "Name: " + this.name, "Quantity: " + this.quantity, "Available until: " +  this.date_to_buy, "Status: " + this.status);
        else
            console.log("ID: " + this.id, "Name: " + this.name, "Quantity: " + this.quantity, "Available until: " +  this.date_to_buy, "Status: " + this.status, "Cost per unit: " + this.cost_per_unit);
    }
}

/**
 * Prints all product from productList
 */
function printAll()
{
    for(let i = 0; i < productList.length; i++)
        productList[i].printProd();
}

/**
 * Generates random int in range <0, max)
 * @param {Number} max 
 * @returns random integer in range <0, max)
 */
function getRandomInt(max)
{
    return Math.floor(Math.random() * max);
}

/**
 * Adds a product to the productList
 * @param {String} name 
 * @param {Number} quantity 
 * @param {String} date_to_buy 
 * @param {String} status 
 * @param {Number} cost_per_unit 
 */
function addProduct(name, quantity, date_to_buy, status, cost_per_unit)
{
    id = getRandomInt(100000);
    const date = new Date(date_to_buy);
    var prod = new Product(id, name, quantity, date, status, cost_per_unit);
    productList.push(prod);
    return id;
}

/**
 * Deletes a product from the productList
 * @param {Number} id 
 */
function delProduct(id)
{
    for(let i = 0; i < productList.length; i++)
        if(id == productList[i].id)
        {
            productList.splice(i, 1);
            return;
        }
}

/**
 * Sets name in product specified by id to new_name
 * @param {Number} id 
 * @param {String} new_name 
 */
function setName(id, new_name)
{
    for(let i = 0; i < productList.length; i++)
        if(id == productList[i].id)
            productList[i].name = new_name;
}

/**
 * Sets status in product specified by id to new_status
 * @param {Number} id 
 * @param {String} new_status 
 */
function setStatus(id, new_status) // zmienić na boola
{
    for(let i = 0; i < productList.length; i++)
        if(id == productList[i].id)
            productList[i].status = new_status;
}

/**
 * Sets quantity in product specified by id to new_quantity
 * @param {Number} id 
 * @param {Number} new_quantity 
 */
function setQuantity(id, new_quantity)
{
    for(let i = 0; i < productList.length; i++)
        if(id == productList[i].id)
            productList[i].quantity = String(new_quantity);
}

/**
 * Sets date in product specified by id to new_date
 * @param {String} id 
 * @param {String} new_date 
 */
function setDate(id, new_date)
{
    for(let i = 0; i < productList.length; i++)
        if(id == productList[i].id)
            productList[i].date_to_buy = new_date;
}

/**
 * Moves product in productList from index from to index to
 * @param {Number} from 
 * @param {Number} to 
 */
function moveProd(from, to)
{
    // to - Start removing from this index
    // 0 - remove 0 items
    // productList.splice(from, 1)[0] - insert this element to index to
    productList.splice(to, 0, productList.splice(from, 1)[0]);
}

function extractDate(date)
{
    let res = "" + date.getFullYear() + date.getDay() + date.getMonth();
    return res;
}

function toBuyToday()
{
    let res = [];

    let today = new Date();
    let extracted = extractDate(today);
    //console.log(extracted);

    for(let i = 0; i < productList.length; i++)
    {
        let tmp = extractDate(productList[i].date_to_buy);
        
        if(tmp == extracted)
            res.push(productList[i]);
    }
    return res;
}

/**
 * Sets new cost_per_unit for product with given id if status == "Bought"
 * @param {Number} id 
 * @param {Number} new_cost 
 */
function setCost(id, new_cost)
{
    for(let i = 0; i < productList.length; i++)
    {
        if(id == productList[i].id && productList[i].status == "Bought")
            productList[i].cost_per_unit = String(new_cost);
    }
}

/**
 * Prints total expenses of the day
 * @param {String} shopping_date 
 * @returns 
 */
function checkExpenses(shopping_date)
{
    let sum = 0;

    for(let i = 0; i < productList.length; i++)
    {
        if(extractDate(shopping_date) == extractDate(productList[i].date_to_buy))
            sum += productList[i].cost_per_unit;
    }
    return sum;
}

/**
 * 
 * @param {List} id_list 
 * @param {Function} foo 
 */
function format(id_list, foo)
{
    for(let i = 0; i < productList.length; i++)
    {
        if(id_list.includes(productList[i].id))
            foo(productList[i]);
    }
}

function foo(product)
{
    product.cost_per_unit *= 4;
}

addProduct("a", "1", "2022-10-18", "False", "10");
addProduct("b", "2", "2022-10-13", "False", "101");
addProduct("c", "2", "2023-01-01", "True");
addProduct("d", "2", "2022-12-01", "True", "2");
addProduct("e", "2", "2025-01-01", "True");
addProduct("f", "2", "2022-12-01", "False", "123123");

delProduct(productList[0].id);

setName(productList[0].id, "aa");

printAll();

//moveProd(1, 3);

setCost(productList[3].id, 1337);

format([productList[0].id, productList[2].id], foo);

console.log("\n\n");
printAll();

console.log(toBuyToday());

console.log(checkExpenses(new Date("2022-12-01")));
