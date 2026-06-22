<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();
if(isset($_POST['opcion'])){ $sistema=$_POST['opcion'];}
else{ $sistema=$_POST['sistema'];}

if(isset($_POST['nuevo'])){
	$sql="insert into actuador values(NULL,".$_POST['pin'].",'".$_POST['tipo']."',".$_POST['sistema'].")";
	$conexion->query($sql);
	}

else if(isset($_POST['borrar'])){
	$sql="delete from actuador where id=".$_POST['borrar'];
	$conexion->query($sql);
	}

else if(isset($_POST['sistema'])){
	$sql="update actuador set pin='".$_POST['pin']."' where id=".$_POST['id'];
	$conexion->query($sql);
	}
 
$sql="select * from actuador where sistema=".$sistema;
$result=$conexion->query($sql);
while($row=mysqli_fetch_array($result)){
	$id=$row['id'];
	$tipo=$row['tipo'];
	$pin=$row['pin'];
	echo $conexion->edit_form($pin,$tipo,0,$sistema,$id,2);
        echo "<form action='editaractuadores.php' method=post><input type=hidden name=borrar value=$id><input type='hidden' name=sistema value =$sistema><input type='submit' value='borrar'></form>";
	echo "------------------------------<br>";
	}

echo $conexion->new_form($sistema,2);
echo "<a href=\"index.php\">Inicio</a> <br>";
