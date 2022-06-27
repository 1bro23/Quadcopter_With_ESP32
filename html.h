#include <Arduino.h>
String html1 = R"foo(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Document</title>)foo";
                    
String html2 = R"foo(
    </head>
    <body>
        <div class="title">
            <h1>Controll</h1>
        </div>
        <div class="main">
            <div class="controll">
                <div class="throtle">
                    <p>Throtle</p>
                    <div class="newColumn">
                        <button class="increaseThrotle">W</button>
                        <button class="decreaseThrotle">S</button>
                    </div>
                    <p><span class="throtleValue">)foo";
int html2_1_throtleValue = 0;
String html2_2 = R"foo(                                                    
                                                    </span> of 4095</p>
                </div>
                <div class="reset">
                    <button class="resetButton">R</button>
                    <span class="resetText"></span>
                </div>
                <div class="direction">
                    <p>Direction</p>
                    <div class="newColumn">
                        <button class="pitchPos">^</button>
                        <div class="newLine">
                            <button class="rollNeg"><</button>
                            <button class="pitchNeg">v</button>
                            <button class="rollPos">></button>
                        </div>
                    </div>
                    <span class="vehicleDirection">Center</span>
                </div>
            </div>
        </div>)foo";
String html3 = R"foo(
    </body>
    </html>
)foo";