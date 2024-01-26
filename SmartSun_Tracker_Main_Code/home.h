// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en" > <!-- Needed for Screenreaders !-->
<head>
<meta http-equiv="refresh" content="2">
<!-- UTF-8 character set covers most characters in the world -->
  <meta charset="utf-8">
  <!-- Make page respond to screen size !-->
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
 
<!--Include a Title. Used by Search Engines -->
<title> Temperature Reading WebServer </title>
<style>
   
   <!--choose good contrast between background and foreground colours -->
   body {       
	background-color: #B2BEB5;
	}
	.flex-Container{
        display: flex;
        flex-direction: column;
        background-color: #B2BEB5;
        align-items: center;
      }
    h1{
    font: bold;
    font-size: 40px;
    font-family: Arial;
    color: black;
	text-align: center;
  }
  p{
    font-size: 25px;
    font-family: Arial;
    color: black;
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
        <td>Humidity</td>
        <td>  )=====");

        
        
String homePagePart3 = F(R"=====(</td>
        <td>Percent</d>
      </table>
     </div>
	</body>
</html>)=====");