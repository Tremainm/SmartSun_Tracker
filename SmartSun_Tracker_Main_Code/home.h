// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.
String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en" > <!-- Needed for Screenreaders !-->
<head>
<!--<meta http-equiv="refresh" content="2">-->
<!-- UTF-8 character set covers most characters in the world -->
  <meta charset="utf-8">
  <!-- Make page respond to screen size !-->
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
 
<!--Include a Title. Used by Search Engines -->
<title> Temperature Reading WebServer </title>
<style>
   
   <!--choose good contrast between background and foreground colours -->
   body {       
	background-color: blue;
	}
	.flex-Container{
        display: flex;
        flex-direction: column;
        background-color: blue;
        align-items: center;
      }
    .flex-keypad{
		display: flex;
       	flex-direction: row;
		background-color: blue;
		align-items: center;
		justify-content:center;
    }

    .button {
        background-color: White;
        margin: 10px;
        padding: 10px;
        font-size: 30px;
        width: 20%;
        text-align: center;
    }
    .button:hover{
	   background-color: yellow;
	   }

    h1{
    font: bold;
    font-size: 40px;
    font-family: Arial;
    color: white;
	text-align: center;
  }
   h2{
    font: bold;
    font-size: 30px;
    font-family: Arial;
    color: white;
	text-align: center;
  }
  p{
    font-size: 25px;
    font-family: Arial;
    color: white;
	text-align: center;
   }  
  th, td {
    font-size: 25px;
    font-family: Arial;
    color: white;
    padding: 8px;
    text-align: left;
    border-bottom: 1px solid #000000;
}
</style>
</head>
	<body>
		<div class="flex-Container">
			<h1> SmartSun Tracker Website </h1> 
			<p>Welcome to my website that tracks the position of the sun to maximize solar energy</p>  
			
			<h2>ThingSpeak Charts</h2>
			<div><iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2412062/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=DHT-11+Temperature&type=line&xaxis=Time&yaxis=Temperature" title="Temperature chart" ></iframe></div>
			<div><iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/2412062/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=DHT-11+Humidity&type=line&xaxis=Time&yaxis=Humidity" title="Humidity chart"></iframe></div>
		<table>
        <tr>
          <th>Sensor</th>
          <th>Value</th>
          <th>Unit</th>
        </tr>
        <tr>
          <td>Temperature</td>
          <td> )=====");

String homePagePart2 = F(R"=====(</td>
			<td>Degree Cel</td>
        </tr>
		<tr>
			<td>Humidity</td>
			<td>  )=====");
        
String homePagePart3 = F(R"=====(</td>
			<td>Percent</d>
		</tr>
      </table>
	  <h2>Servo Control</h2>
	  <p>Use the buttons to manually control the servos</p
	  <div class = "flex-keypad">
		<div class = "button" onclick="fetch('/sentKeyPressToWebServer?button=W')">Up</div>
	  </div>
	  <div class = "flex-keypad">
		<div class = "button"  onclick="fetch('/sentKeyPressToWebServer?button=A')">Left</div>
		<div class = "button"  onclick="fetch('/sentKeyPressToWebServer?button=D')">Right</div>
     </div>
	 <div class = "flex-keypad">
		<div class = "button"  onclick="fetch('/sentKeyPressToWebServer?button=S')">Down</div>
	 </div>
	 </div>
	</body>
</html>)=====");

