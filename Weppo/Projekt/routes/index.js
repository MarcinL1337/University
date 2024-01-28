var express = require("express");
var router = express.Router();
var Product = require("../models/product");
var csrf = require("csurf");

var csrfProtection = csrf();
router.use(csrfProtection);

/* GET home page. */
router.get("/", async (req, res, next) => {
  var products = await Product.find().lean(); // .lean() -> get a JSON object instead of a mongoose one
  res.render("shop/index", { title: "Sneakers Shop", products: products });
});

router.get("/user/signup", function (req, res, next) {
  res.render("user/signup", { csrfToken: req.csrfToken() });
});

router.post("/user/signup", function (req, res, next) {
  res.redirect("/");
});

module.exports = router;
