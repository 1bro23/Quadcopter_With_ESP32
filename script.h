#include <Arduino.h>
String script = R"foo(
    <script>
let button,text;
//keydown event
document.addEventListener("keydown",e=>{
    //if key w press
    if(e.key=="w"){
        button = document.querySelector(".increaseThrotle")
        button.classList.add("active")
        //fetch
        fetch("http://192.168.1.100/gainSpeed").then(e=>e.text()).then(e=>{
            console.log(e)
            //change throtle value (increase)
            text = document.querySelector(".throtleValue");
            text.innerHTML = e;
        }) 
    }
    //if key s press
    if(e.key=="s"){
        button = document.querySelector(".decreaseThrotle")
        button.classList.add("active")
        //fetch
        fetch("http://192.168.1.100/decreaseSpeed").then(e=>e.text()).then(e=>{
            console.log(e)
            //change throtle value (increase)
            text = document.querySelector(".throtleValue");
            text.innerHTML = e;
        })
    }
})
//keypress event
document.addEventListener("keydown",e=>{
    if(!e.repeat){
        //if arrow press
        if(e.key=="ArrowUp"){
            button = document.querySelector(".pitchPos")
            button.classList.add("active");
            fetch("http://192.168.1.100/pitchPos").then(e=>e.text()).then(e=>{
                console.log(e)
            })
            text = document.querySelector(".vehicleDirection")
            text.innerHTML = "Pitch +"
        }
        if(e.key=="ArrowRight"){
            button = document.querySelector(".rollPos")
            button.classList.add("active");
            fetch("http://192.168.1.100/rollPos").then(e=>e.text()).then(e=>{
                console.log(e)
            })
            text = document.querySelector(".vehicleDirection")
            text.innerHTML = "Roll +"
        }
        if(e.key=="ArrowDown"){
            button = document.querySelector(".pitchNeg")
            button.classList.add("active");
            fetch("http://192.168.1.100/pitchNeg").then(e=>e.text()).then(e=>{
                console.log(e)
            })
            text = document.querySelector(".vehicleDirection")
            text.innerHTML = "Pitch -"
        }
        if(e.key=="ArrowLeft"){
            button = document.querySelector(".rollNeg")
            button.classList.add("active");
            fetch("http://192.168.1.100/rollNeg").then(e=>e.text()).then(e=>{
                console.log(e)
            })
            text = document.querySelector(".vehicleDirection")
            text.innerHTML = "Roll -"
        }
        if(e.key=="r"){
            button = document.querySelector(".resetButton")
            button.classList.add("active");
            fetch("http://192.168.1.100/reset").then(e=>e.text()).then(e=>{
                console.log(e)
            })
            text = document.querySelector(".resetText")
            text.innerHTML = "Reseted"
        }
    }
})
//keyup event
document.addEventListener("keyup",e=>{
    //if key w press
    if(e.key=="w"){
        button = document.querySelector(".increaseThrotle")
        button.classList.remove("active")
    }
    //if key s press
    if(e.key=="s"){
        button = document.querySelector(".decreaseThrotle")
        button.classList.remove("active")
    }
    //if arrow press
    if(e.key=="ArrowUp"){
        button = document.querySelector(".pitchPos")
        button.classList.remove("active");
        fetch("http://192.168.1.100/stable").then(e=>e.text()).then(e=>{
            console.log(e)
        })
        text = document.querySelector(".vehicleDirection")
        text.innerHTML = "Center"
    }
    if(e.key=="ArrowRight"){
        button = document.querySelector(".rollPos")
        button.classList.remove("active");
        fetch("http://192.168.1.100/stable").then(e=>e.text()).then(e=>{
            console.log(e)
        })
        text = document.querySelector(".vehicleDirection")
        text.innerHTML = "Center"
    }
    if(e.key=="ArrowDown"){
        button = document.querySelector(".pitchNeg")
        button.classList.remove("active");
        fetch("http://192.168.1.100/stable").then(e=>e.text()).then(e=>{
            console.log(e)
        })
        text = document.querySelector(".vehicleDirection")
        text.innerHTML = "Center"
    }
    if(e.key=="ArrowLeft"){
        button = document.querySelector(".rollNeg")
        button.classList.remove("active");
        fetch("http://192.168.1.100/stable").then(e=>e.text()).then(e=>{
            console.log(e)
        })
        text = document.querySelector(".vehicleDirection")
        text.innerHTML = "Center"
    }
    if(e.key=="r"){
        button = document.querySelector(".resetButton")
        button.classList.remove("active");
        text = document.querySelector(".resetText")
        setTimeout(()=>{
            text.innerHTML = ""
        },500)
    }
})
    </script>
)foo";