<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();
$cantidadmostrar=30;
$compag=(!isset($_GET['pag'])) ? 1: $_GET['pag'];

echo "MONITOREO <br>";
echo $conexion->make_link("monitoreo.php?opcion=1","Sensores",1);
echo $conexion->make_link("monitoreo.php?opcion=2","Actuadores",2);

if(isset($_GET['opcion'])){
	$opc=$_GET['opcion'];
	$indate=$conexion->select_fecha($_GET['fechaini'],0);
	$endate=$conexion->select_fecha($_GET['fechater'],1);
	if($opc==1){ $types=$conexion->select_type('sensor','tipo',$_GET['tipo']);}
	else{$types=$conexion->select_type('actuador','tipo',$_GET['tipo']);}
	$formulario=$conexion->all_systems($_GET['sistema']);
	$tipomonitoreo="<form action='monitoreo.php' method=get>sistema $formulario dato $types Inicio $indate Final $endate <input type=hidden name=opcion value=$opc><input type=submit value='ver registros'></form>";
	echo $tipomonitoreo;
	}

if(isset($_GET['tipo'])){
	if($opc==1){
		$sql="select fecha,hora,valor from monitoreo_sensor where fecha>='".$_GET['fechaini']."' and fecha<='".$_GET['fechater']."' and id_sensor=(select id from sensor where tipo='".$_GET['tipo']."' and sistema=".$_GET['sistema'].")";
	        $cond="where fecha>='".$_GET['fechaini']."' and fecha<='".$_GET['fechater']."' and id_sensor=(select id from sensor where tipo='".$_GET['tipo']."' and sistema=".$_GET['sistema'].") order by fecha desc,hora desc limit ".(($compag-1)*$cantidadmostrar).",".$cantidadmostrar;
		$campos=array('fecha','hora','valor');
		$table=$conexion->query_table($campos,$campos,'monitoreo_sensor',$cond);
		}
	if($opc==2){
		$sql="select fecha,hora,accion from monitoreo_actuador where fecha>='".$_GET['fechaini']."' and fecha<='".$_GET['fechater']."' and id_actuador=(select id from actuador where tipo='".$_GET['tipo']."' and sistema=".$_GET['sistema'].")";
	        $cond="where fecha>='".$_GET['fechaini']."' and fecha<='".$_GET['fechater']."' and id_actuador=(select id from actuador where tipo='".$_GET['tipo']."' and sistema=".$_GET['sistema'].") order by fecha desc,hora desc limit ".(($compag-1)*$cantidadmostrar).",".$cantidadmostrar;
		$campos=array('fecha','hora','accion');
		$table=$conexion->query_table($campos,$campos,'monitoreo_actuador',$cond);
		}
	echo $table;
	$registros=$conexion->query($sql);
	$totalregistros=ceil($registros->num_rows/$cantidadmostrar);
	$increment=(($compag +1) <= $totalregistros) ? ($compag +1): $totalregistros;
	$decrement=(($compag -1) < 1) ? 1: ($compag-1);
	echo "<div><ul><a href=\"?pag=".$decrement."&tipo=".$_GET['tipo']."&fechaini=".$_GET['fechaini']."&fechater=".$_GET['fechater']."&opcion=".$_GET['opcion']."&sistema=".$_GET['sistema']."\">atras  </a>";
	$desde=$compag - 4;
	$hasta=$compag +5;
	$desde=($desde<1)? 1 : $desde;
	$hasta=($hasta>$totalregistros)? $totalregistros : $hasta;
	for($i=$desde;$i<=$hasta;$i++){
		if($i==$compag){ echo "<a href=\"?pag=".$i."&tipo=".$_GET['tipo']."&fechaini=".$_GET['fechaini']."&fechater=".$_GET['fechater']."&opcion=".$_GET['opcion']."&sistema=".$_GET['sistema']."\"> (".$i.") </a>";}
		else{ echo "<a href=\"?pag=".$i."&tipo=".$_GET['tipo']."&fechaini=".$_GET['fechaini']."&fechater=".$_GET['fechater']."&opcion=".$_GET['opcion']."&sistema=".$_GET['sistema']."\"> (".$i.") </a>";}
		}
	echo "<a href=\"?pag=".$decrement."&tipo=".$_GET['tipo']."&fechaini=".$_GET['fechaini']."&fechater=".$_GET['fechater']."&opcion=".$_GET['opcion']."&sistema=".$_GET['sistema']."\">  siguiente</a></ul></div>";
	}

echo "<a href=\"index.php\">Inicio</a> <br>";

?>

