#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-Tetris";
const char* password = "12345678";

WebServer server(80);

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Tetris</title>

<style>
body{
    background:#111;
    color:white;
    text-align:center;
    font-family:Arial;
}
canvas{
    border:3px solid white;
    background:black;
}
h1{
    color:cyan;
}
</style>
</head>

<body>

<h1>ESP32 Tetris</h1>

<canvas id="game" width="240" height="400"></canvas>

<script>

const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');

ctx.scale(20,20);

const arena = createMatrix(12,20);

const colors = [
 null,
 '#FF0D72',
 '#0DC2FF',
 '#0DFF72',
 '#F538FF',
 '#FF8E0D',
 '#FFE138',
 '#3877FF'
];

const player = {
 pos:{x:0,y:0},
 matrix:null
};

function createMatrix(w,h){
 const matrix=[];
 while(h--){
  matrix.push(new Array(w).fill(0));
 }
 return matrix;
}

function createPiece(type){

 if(type==='T'){
  return [
   [0,0,0],
   [1,1,1],
   [0,1,0]
  ];
 }

 else if(type==='O'){
  return [
   [2,2],
   [2,2]
  ];
 }

 else if(type==='L'){
  return [
   [0,3,0],
   [0,3,0],
   [0,3,3]
  ];
 }

 else if(type==='J'){
  return [
   [0,4,0],
   [0,4,0],
   [4,4,0]
  ];
 }

 else if(type==='I'){
  return [
   [0,5,0,0],
   [0,5,0,0],
   [0,5,0,0],
   [0,5,0,0]
  ];
 }

 else if(type==='S'){
  return [
   [0,6,6],
   [6,6,0],
   [0,0,0]
  ];
 }

 else if(type==='Z'){
  return [
   [7,7,0],
   [0,7,7],
   [0,0,0]
  ];
 }

}

function drawMatrix(matrix,offset){

 matrix.forEach((row,y)=>{
  row.forEach((value,x)=>{

   if(value!==0){

    ctx.fillStyle=colors[value];
    ctx.fillRect(x+offset.x,y+offset.y,1,1);

   }

  });
 });

}

function draw(){

 ctx.fillStyle='#000';
 ctx.fillRect(0,0,canvas.width,canvas.height);

 drawMatrix(arena,{x:0,y:0});
 drawMatrix(player.matrix,player.pos);

}

function merge(arena,player){

 player.matrix.forEach((row,y)=>{

  row.forEach((value,x)=>{

   if(value!==0){

    arena[y+player.pos.y][x+player.pos.x]=value;

   }

  });

 });

}

function collide(arena,player){

 const [m,o]=[player.matrix,player.pos];

 for(let y=0;y<m.length;++y){

  for(let x=0;x<m[y].length;++x){

   if(
    m[y][x]!==0 &&
    (arena[y+o.y] &&
    arena[y+o.y][x+o.x])!==0
   ){
    return true;
   }

  }

 }

 return false;

}

function playerDrop(){

 player.pos.y++;

 if(collide(arena,player)){

  player.pos.y--;

  merge(arena,player);

  playerReset();

  arenaSweep();

 }

 dropCounter=0;

}

function playerMove(dir){

 player.pos.x+=dir;

 if(collide(arena,player)){

  player.pos.x-=dir;

 }

}

function rotate(matrix){

 for(let y=0;y<matrix.length;++y){

  for(let x=0;x<y;++x){

   [
    matrix[x][y],
    matrix[y][x]
   ]=[
    matrix[y][x],
    matrix[x][y]
   ];

  }

 }

 matrix.forEach(row=>row.reverse());

}

function playerRotate(){

 rotate(player.matrix);

 if(collide(arena,player)){

  rotate(player.matrix);
  rotate(player.matrix);
  rotate(player.matrix);

 }

}

function arenaSweep(){

 outer:
 for(let y=arena.length-1;y>0;--y){

  for(let x=0;x<arena[y].length;++x){

   if(arena[y][x]===0){

    continue outer;

   }

  }

  const row=arena.splice(y,1)[0].fill(0);

  arena.unshift(row);

  ++y;

 }

}

function playerReset(){

 const pieces='TJLOSZI';

 player.matrix=createPiece(
  pieces[pieces.length*Math.random()|0]
 );

 player.pos.y=0;

 player.pos.x=
 (arena[0].length/2|0)-
 (player.matrix[0].length/2|0);

 if(collide(arena,player)){

  arena.forEach(row=>row.fill(0));

 }

}

let dropCounter=0;
let dropInterval=500;
let lastTime=0;

function update(time=0){

 const deltaTime=time-lastTime;

 lastTime=time;

 dropCounter+=deltaTime;

 if(dropCounter>dropInterval){

  playerDrop();

 }

 draw();

 requestAnimationFrame(update);

}

document.addEventListener('keydown',event=>{

 if(event.keyCode===37){

  playerMove(-1);

 }

 else if(event.keyCode===39){

  playerMove(1);

 }

 else if(event.keyCode===40){

  playerDrop();

 }

 else if(event.keyCode===38){

  playerRotate();

 }

});

playerReset();
update();

</script>

</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  WiFi.setSleep(false);

  IPAddress IP = WiFi.softAPIP();

  Serial.print("IP Address: ");
  Serial.println(IP);

  server.on("/", handleRoot);

  server.begin();

  Serial.println("Server Started");

}

void loop() {

  server.handleClient();

}
