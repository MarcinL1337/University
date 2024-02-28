function Foo() {
  var Qux = function () {
    console.log("To jest funkcja prywatna Qux");
  };

  this.Bar = function () {
    console.log("To jest metoda publiczna Bar");
    Qux();
  };
}

var instance = new Foo();

instance.Bar(); 

// instance.Qux();