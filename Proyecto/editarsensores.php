<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();
if(isset($_POST['opcion'])){ $sistema=$_POST['opcion'];}
else{ $sistema=$_POST['sistema'];}

if(isset($_POST['nuevo'])){
	$sql="insert into sensor values(NULL,".$_POST['pin'].",'".$_POST['tipo']."',".$_POST['muestreo'].",".$_POST['sistema'].")";
	$conexion->query($sql);
	}

else if(isset($_POST['borrar'])){
	$sql="delete from sensor where id=".$_POST['borrar'];
	$conexion->query($sql);
	}

else if(isset($_POST['sistema'])){
	$sql="update sensor set pin='".$_POST['pin']."', muestreo='".$_POST['muestreo']."' where id=".$_POST['id'];
	$conexion->query($sql);
	}
 
$sql="select * from sensor where sistema=".$sistema;
$result=$conexion->query($sql);
while($row=mysqli_fetch_array($result)){
	$id=$row['id'];
	$tipo=$row['tipo'];
	$pin=$row['pin'];
	$muestreo=$row['muestreo'];
	echo $conexion->edit_form($pin,$tipo,$muestreo,$sistema,$id,1);
        echo "<form action='editarsensores.php' method=post><input type=hidden name=borrar value=$id><input type='hidden' name=sistema value =$sistema><input type='submit' value='borrar'></form>";
	echo "------------------------------<br>";
	}

echo $conexion->new_form($sistema,1);
echo "<a href=\"index.php\">Inicio</a> <br>";
