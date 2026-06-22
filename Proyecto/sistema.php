<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

if(isset($_POST['nuevo'])){
	$sql="insert into sistema values(NULL,'".$_POST['nombre']."','".$_POST['ubicacion']."','".$_POST['url']."',".$_POST['planificador'].",1,".$_POST['padre'].")";
	$conexion->query($sql);
	echo "Sistema creado <br>";
	}

else if(isset($_POST['start'])){
        $command="bash -c 'exec -a ".$_POST['nombre']." /home/Hidroponia/Hidroponia ".$_POST['start']."'";
        echo $command."<br>";
	system($command);
        }

else if(isset($_POST['borrar'])){
        $cmd="./kill ".$_POST['borrar']." ".$_POST['nombre'];
        system($cmd);
        $conexion->borrar_sys($_POST['borrar']);
	echo "Sistema borrado <br>";
        }

else if(isset($_POST['edit'])){
        $cmd="./kill ".$_POST['id']." ".$_POST['nombre'];
        system($cmd);
	$sql="update sistema 
		set nombre='".$_POST['nombre']."', ubicacion='".$_POST['ubicacion']."', url='".$_POST['url']."', id_planificador='".$_POST['planificador']."', 
		estado='".$_POST['estado']."', parent='".$_POST['padre']."' where id=".$_POST['id'];
	echo $sql."<br>";
	$conexion->query($sql);
	echo "Sistema editado <br>";
	echo "launch <br>";
	}

else if(isset($_POST['opcion'])){
	if($_POST['opcion']==0){
		echo "CREAR NUEVO<br>";
		echo $conexion->make_link("sistema.php","Sistema",1);
        	echo $conexion->make_link("sistema.php","Subsistema",2);
		}
	if($_POST['opcion']==1){ echo $conexion->new_sys();}
	if($_POST['opcion']==2){ echo $conexion->new_subsys();}
	}

else if(isset($_POST['sistema'])){
        $id = $_POST['sistema'];
        $padre= $_POST['parent'];
        $info="select * from sistema where id=".$id;
        $res=$conexion->query($info);
        while($row=mysqli_fetch_array($res)){
                $nombre=$row['nombre'];
                $ubicacion=$row['ubicacion'];
                $url=$row['url'];
                $planificador=$row['id_planificador'];
                $estado=$row['estado'];
                $padre=$row['parent']; 
                if($row['parent']==0){  echo "SISTEMA ".$row['nombre'];}
                else{ echo "SUBSISTEMA ".$row['nombre'];}
                echo "<br> <br>";}
        if($padre==0){ echo $conexion->system_form($nombre,$ubicacion,$url,$planificador,$id);}
	else{ echo $conexion->subsys_form($nombre,$ubicacion,$url,$planificador,$padre,$id);}
        echo $conexion->make_link("editarsensores.php","editar sensores",$id);
        echo $conexion->make_link("editaractuadores.php","editar actuadores",$id);
        echo "<form action='sistema.php' method=post><input type=hidden name=start value=$id><input type=hidden name=nombre value=$nombre><input type='submit' value='start'></form>";
        echo "<form action='sistema.php' method=post><input type=hidden name=borrar value=$id><input type=hidden name=nombre value=$nombre><input type='submit' value='borrar'></form>";
        if($padre==0){
                echo "--------Subsistemas: ";
                echo $conexion->escoger_sistema("sistema.php",$id);
                }
        }

$formulario=$conexion->escoger_sistema("sistema.php",0);
$registro=$conexion->make_link("sistema.php","Nuevo sistema",0);

if(!isset($_POST['sistema']) and !isset($_POST['opcion'])){
	echo "------Escoger sistema<br>";
	echo $formulario;
	echo $registro;
	}
echo "<a href=\"index.php\">Inicio</a> <br>";

?>
