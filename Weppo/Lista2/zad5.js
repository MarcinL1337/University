let obj = {
    property1: 1,
    property2: "2",

    get property1() {
        return this.property1;
    },
    set property1(val){
        this.property1 = val;
    }
}

console.log(obj);


obj["property3"] = "property3";

obj.method = function() {console.log(this.property3)};

console.log(obj);

obj.method();

let property4_val = 420;

// Nowe gettery i settery w obiekcie można ustawić tylko za pomocą defineProperty

Object.defineProperty(obj, "property4", {
    get() {
        return property4_val;
    },

    set(val) {
        property4_val = val;
    }
});


console.log(obj.property4);
obj.property4 = 1337;
console.log(obj.property4);
