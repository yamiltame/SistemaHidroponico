<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

if(isset($_POST['nuevo'])){
	$sql="insert into planificador values(NULL,".$_POST['riego'].",'".$_POST['cultivo']."',".$_POST['phmax'].",".$_POST['phmin'].",".$_POST['hmax'].",".$_POST['hmin'].",".$_POST['temp'].",".$_POST['rango'].")";
	$conexion->query($sql);
	echo "Sistema creado <br>";
	}

else if(isset($_POST['borrar'])){
        $sql="delete from planificador where id=".$_POST['borrar'];
        $conexion->query($sql);
	echo "Sistema borrado <br>";
        }

else if(isset($_POST['edit'])){
	$sql="update planificador set id_riego='".$_POST['riego']."', cultivo='".$_POST['cultivo']."', phmax='".$_POST['phmax']."', phmin='".$_POST['phmin']."', hmax='".$_POST['hmax']."', hmin='".$_POST['hmin']."', temperatura_ambiente='".$_POST['temp']."', rango_ambiente='".$_POST['rango']."' where id=".$_POST['id'];
	$conexion->query($sql);
	echo "Planificador editado <br>";
	}

else if(isset($_POST['opcion'])){
	echo $conexion->new_plan();
	echo "----------------------------------------<br>";
	}

else if(isset($_POST['planificador'])){
	$id = $_POST['planificador'];
	$info="select * from planificador where id=".$id;
	$res=$conexion->query($info);
	while($row=mysqli_fetch_array($res)){
		$riego=$row['id_riego'];
		$cultivo=$row['cultivo'];
		$phmax=$row['phmax'];
		$phmin=$row['phmin'];
		$hmax=$row['hmax'];
		$hmin=$row['hmin'];
		$temperatura_ambiente=$row['temperatura_ambiente'];
		$rango_ambiente=$row['rango_ambiente'];
		}
	echo $conexion->edit_plan($riego,$cultivo,$phmax,$phmin,$hmax,$hmin,$temperatura_ambiente,$rango_ambiente,$id);
        echo "<form action='planificador.php' method=post><input type=hidden name=borrar value=$id><input type='submit' value='borrar'></form>";
	echo "-----------------------------------------<br>";
	}

$formulario="<form action='planificador.php' method='post'>".$conexion->escoger_plan(0)."<input type=submit value='Editar Planificacion'></form>";
$registro=$conexion->make_link("planificador.php","Nueva planificacion",1);
echo "PLANIFICADOR<br>";
echo $formulario;
echo $registro;
echo "<a href=\"index.php\">Inicio</a> <br>";

?>
