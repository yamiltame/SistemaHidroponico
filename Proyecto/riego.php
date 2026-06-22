<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

if(isset($_POST['nuevo'])){
	$sql="insert into riego values(NULL,'".$_POST['tipo']."',".$_POST['t_nor'].",".$_POST['p_nor'].",".$_POST['Te_nor'].",".$_POST['t_mas'].",".$_POST['p_mas'].",".$_POST['Te_mas'].",".$_POST['t_men'].",".$_POST['p_men'].",".$_POST['Te_men'].")";
	$conexion->query($sql);
	echo "Riego creado <br>";
	}

else if(isset($_POST['borrar'])){
        $sql="delete from riego where id=".$_POST['borrar'];
	$conexion->query($sql);
	echo "Riego borrado <br>";
        }

else if(isset($_POST['edit'])){
	$sql="update riego set tipo='".$_POST['tipo']."', t_nor='".$_POST['t_nor']."', p_nor='".$_POST['p_nor']."', t_mas='".$_POST['t_mas']."', p_mas='".$_POST['p_mas']."', Te_mas='".$_POST['Te_mas']."', t_men='".$_POST['t_men']."', p_men='".$_POST['p_men']."', Te_men='".$_POST['Te_men']."' where id=".$_POST['id'];
	$conexion->query($sql);
	echo $sql."<br>";
	echo "Riego editado <br>";
	}

else if(isset($_POST['opcion'])){
	echo $conexion->new_riego();
	echo "----------------------------------------<br>";
	}

else if(isset($_POST['riego'])){
	$id = $_POST['riego'];
	$info="select * from riego where id=".$id;
	$res=$conexion->query($info);
	while($row=mysqli_fetch_array($res)){
		$tipo=$row['tipo'];
		$t_nor=$row['t_nor'];
		$p_nor=$row['p_nor'];
		$Te_nor=$row['Te_nor'];
		$t_mas=$row['t_mas'];
		$p_mas=$row['p_mas'];
		$Te_mas=$row['Te_mas'];
		$t_men=$row['t_men'];
		$p_men=$row['p_men'];
		$Te_men=$row['Te_men'];
		}
	echo $conexion->edit_riego($tipo,$t_nor,$p_nor,$Te_nor,$t_mas,$p_mas,$Te_mas,$t_men,$p_men,$Te_men,$id);
        echo "<form action='riego.php' method=post><input type=hidden name=borrar value=$id><input type='submit' value='borrar'></form>";
	echo "-----------------------------------------<br>";
	}

$formulario="<form action='riego.php' method='post'>".$conexion->select_riego(0)."<input type=submit value='Editar Riego'></form>";
$registro=$conexion->make_link("riego.php","Nuevo Riego",1);
echo "RIEGO<br>";
echo $formulario;
echo $registro;
echo "<a href=\"index.php\">Inicio</a> <br>";

?>
