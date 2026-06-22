<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

$Time="<select name='time'>
	<option value=0> Hora </option>
	<option value=1> Dia </option>
	<option value=2> Semana </option>
	<option value=3> Mes </option>
	</select>";
$indate=$conexion->select_fecha($_POST['fechaini'],0);
$endate=$conexion->select_fecha($_POST['fechater'],1);
$types=$conexion->select_type('sensor','tipo',$_POST['tipo']);
$tipoestadistica="<form action='filling.php' method=post> Tipo estadistica $Time  Dato $types Inicio $indate Final $endate <input type=submit value='generar'></form>";
echo $tipoestadistica;
$horas=array('00','01','02','03','04','05','06','07','08','09','10','11','12','13','14','15','16','17','18','19','20','21','22','23');
if(isset($_POST['time'])){
	$opc=$_POST['time'];
	if($opc==0){
		echo "ESTADISTICA POR HORA <br>La tabla muestra el promedio por hora de cada dia<br>";
		$sistemas=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
		while($row=mysqli_fetch_array($sistemas)){
			$sql="select distinct fecha from monitoreo_sensor where fecha>='".$_POST['fechaini']."' and fecha<='".$_POST['fechater']."'";
			$result=$conexion->query($sql);
			while($row2=mysqli_fetch_array($result)){
				$query="insert into estadistica_hora values ('".$row2['fecha']."','".$_POST['tipo']."',".$row['sistema'].",";
				foreach($horas as $h){
					$rprom=$conexion->query("select avg(valor) as promedio from monitoreo_sensor where id_sensor=(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$row2['fecha']."' and hora like '$h%'");
					$riw=mysqli_fetch_array($rprom);
					$prom=number_format($riw['promedio'],2,".","");
//					echo $prom."<br>";
					if($h=='23'){$query.=$prom.")";}
					else{ $query.=$prom.",";}
					}
				echo $query."<br>";
				$conexion->query($query);
				echo "inserted ".$row2['fecha']." ".$_POST['tipo']." ".$row['sistema']."<br>";
				}
			$res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
			$sis=mysqli_fetch_array($res);
			echo $sis['nombre'];
			}
		}
	if($opc==1){
		$form="<form action='estadisticas.php' method=post>Dato $types  Inicio $indate Final $endate <input type=hidden name='opcion' value=$opc><input type=submit value='generar'><br></form>";
		echo "Estadistica por dia<br>";
		echo $form;
		}
	if($opc==2){
		$form="<form action='estadisticas.php' method=post>Dato $types  Inicio $indate Final $endate <input type=submit value='generar'><br></form>";
		echo "Estadistica por semana<br>";
		echo $form;
		}
	if($opc==3){
		$form="<form action='estadisticas.php' method=post>Dato $types  Inicio $indate Final $endate <input type=submit value='generar'><br></form>";
		echo "Estadistica por mes<br>";
		echo $form;
		}
	}

/*if(isset($_POST['tipo'])){
	$result=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
	while($row=mysqli_fetch_array($result)){
		$cond="where id_sensor=(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$_POST['fechaini']."' order by id_monitoreo desc";
		$tabla=$conexion->query_table(array('fecha','hora','valor'),'monitoreo_sensor',$cond);
		$res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
		$sis=mysqli_fetch_array($res);
	       	$re=$conexion->query("select avg(valor) as promedio from monitoreo_sensor where id_sensor =(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$_POST['fechaini']."'");
		$riw=mysqli_fetch_array($re);
		$avg=$riw['promedio'];
	       	$re=$conexion->query("select std(valor) as desvstd from monitoreo_sensor where id_sensor =(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$_POST['fechaini']."'");
		$riw=mysqli_fetch_array($re);
		$std=$riw['desvstd'];
	       	$re=$conexion->query("select variance(valor) as vari from monitoreo_sensor where id_sensor =(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$_POST['fechaini']."'");
		$riw=mysqli_fetch_array($re);
		$variance=$riw['vari'];
		echo $_POST['tipo']." del sistema ".$sis['nombre']."<br> ".$tabla."Promedio del dia: ".$avg."<br> Desviacion standar: ".$std."<br> Varianza : ".$variance."<br><br>";
		}
	}*/
echo "<a href=\"index.php\">Inicio</a> <br>";

?>

