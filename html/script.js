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

//pMinButton
const pMinButton  = document.querySelector(".proportional .min");
const pPlusButton  = document.querySelector(".proportional .plus");
const iMinButton  = document.querySelector(".integral .min");
const iPlusButton  = document.querySelector(".integral .plus");
const dMinButton  = document.querySelector(".derivative .min");
const dPlusButton  = document.querySelector(".derivative .plus");
//proportional
pMinButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/MinProportional").then(e=>e.text()).then(e=>{
            document.querySelector(".proportional span").innerHTML = e;
        })
})
pPlusButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/PlusProportional").then(e=>e.text()).then(e=>{
            document.querySelector(".proportional span").innerHTML = e;
        })
})
//integral
iMinButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/MinIntegral").then(e=>e.text()).then(e=>{
            document.querySelector(".integral span").innerHTML = e;
        })
})
iPlusButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/PlusIntegral").then(e=>e.text()).then(e=>{
            document.querySelector(".integral span").innerHTML = e;
        })
})
//derivative
dMinButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/MinDerivative").then(e=>e.text()).then(e=>{
            document.querySelector(".derivative span").innerHTML = e;
        })
})
dPlusButton.addEventListener("click",()=>{
    fetch("http://192.168.1.100/PlusDerivative").then(e=>e.text()).then(e=>{
            document.querySelector(".derivative span").innerHTML = e;
        })
})