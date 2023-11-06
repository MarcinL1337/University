function Tree(val, left, right) {
  this.left = left;
  this.right = right;
  this.val = val;
}

Tree.prototype[Symbol.iterator] = function* () {
  let queue = [this]; 

  while (queue.length > 0) {
    const currentNode = queue.shift();
    yield currentNode.val; 

    if (currentNode.right) {
      queue.push(currentNode.right); 
    }

    if (currentNode.left) {
      queue.push(currentNode.left); 
    }
  }
};

var root = new Tree(1, new Tree(2, new Tree(3)), new Tree(4));

for (var e of root) {
  console.log(e);
}
