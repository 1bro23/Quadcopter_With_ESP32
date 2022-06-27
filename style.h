#include <Arduino.h>
String style = R"foo(
    <style>
        *{
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body{
    width: 100vw;
    height: 100vh;
    background-color: black;
    
    display: flex;
    flex-direction: column;
}
div{
    display: flex;
    align-items: center;
    justify-content: center;
}
button{
    width: 30px;
    height: 20px;
}



.title{
    width: 100vw;
    height: 100px;
    background-color: aqua;
    
    flex-direction: column;
}
.main{
    width: 100vw;
    height: 100%;
    background-color: pink;
    
    flex-direction: column;
}
.controll{
    width: 100vw;
    height: 100px;
    background-color: black;

    justify-content: space-around;
}
.throtle{
    width: 100px;
    height: 100px;
    background-color: aqua;

    flex-direction: column;
    justify-content: space-around;
}
.direction{
    width: 100px;
    height: 100px;
    background-color: aqua;

    flex-direction: column;
    justify-content: space-around;
}
.newColumn{
    flex-direction: column;
}
.active{
    background-color: rgba(100, 100, 100, 0.9);
}
.reset{
    width: 100px;
    height: 100px;
    background-color: aqua;

    flex-direction: column;
}
/* debug */
h1{  
    background-color: blue;
}
/* ------------------------- */
    </style>
)foo";