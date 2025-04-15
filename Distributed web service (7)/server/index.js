const express = require("express");
const app = express()
const response = [
    {
        name: "Manish",
        email: "manish@gmail.com"
    },
    {
        name: "Zabhi",
        email: "zabhi@gmail.com"
    },
    {
        name: "adhwaith",
        email: "adhwaith@gmail.com"
    },
];
app.get("/users", (req, res) => {
    res.json(response);
})


app.listen(5000, () => {
    console.log("listening on port 5000");
})