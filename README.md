<!-- PROJECT LOGO -->
<br />
<p align="center">
</p>
  <h3 align="center">Cobalt.Solutions powned </h3>
  <p align="center">
    <br />
    <br />
</p>


## CREDITS 
Before starting i would like to thanks:

aav666 -> gave me loader and an anydesk to test their shit on :)
<!-- TABLE OF CONTENTS -->
## Table of Contents

* [Why The leak](#why-the-leak)
* [Pasted](#pasted)
* [Crack](#Crack)
* [How to use](#usage)

<!-- ABOUT THE PROJECT -->
## Why the leak

https://www.elitepvpers.com/forum/call-duty/5022604-exposing-cobalt-solutions.html
https://cdn.discordapp.com/attachments/1020007679479005205/1031364093845712926/Screenshot_20221016_180751.jpg


-> got banned in 2 games
-> shit ass rat

<!-- GETTING STARTED -->
## Pasted

Blackbone imported into their loader, vmp'ed dll & kernel LOL

### Crack

So, How did i do ? 

I took their loader which was unprotected and started analyzing 

They used imgui so finding the main gui thread wasn't a hard thing to achieve. 
After i found their imgui i tried to replicate products list, but unfortunately they used server to stream dll
But i turned on my brain and found out they use the same function to change 'state' for their imgui to render. 

With literally 2 braincells, i found out the functions ChangeStateWithMessage (A2CF0)

<img src=https://cdn.discordapp.com/attachments/898252526456012871/1031742446742274129/unknown.png></img>

With this function in my hand, i just used _ReturnAddress() to spot most of functions that was used by cobalt itself; so just by running their loader and injecting, i was able to retrace every functions, such as map&run

<img src=https://cdn.discordapp.com/attachments/898252526456012871/1031742873202327562/unknown.png></img>

Now that we have the main function to inject into an game process, it was kinda simple to reproduce their loader, just by dumping dll data & putting it to a file. 

The sames goes with kernel, but it was even simplier, even if I spotted kdmapper by following changestate, i could have used **strings** yeah, they didn't even xor'ed kdmapper strings

<img src=https://cdn.discordapp.com/attachments/898252526456012871/1031743322093539408/unknown.png></img>

This prove you how amateur they are. 

To make a real "crack" I decided to simply reproduce their loader with hooks and code, for my own protections and yours (<3) I blocked their network connections! 
which means they didn't even have a killswitch or even network check in injection step !! 

Source code can be found in the folder CrackSource/ and the "release" version of the crack in Release/ 


Enjoy ;) and remember ! STOP PASTING (and putting virus in your loader), thanks <3
