<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

$Time="<select name='time'>";
if($_POST['time']==0){
	$Time.="<option value=0 selected> Hora </option>
		<option value=1> Dia </option>
		<option value=2> Semana </option>
		<option value=3> Mes </option>
		</select>";
	}
else if($_POST['time']==1){
	$Time.="<option value=0> Hora </option>
		<option value=1 selected> Dia </option>
		<option value=2> Semana </option>
		<option value=3> Mes </option>
		</select>";
	}
else if($_POST['time']==2){
	$Time.="<option value=0> Hora </option>
		<option value=1> Dia </option>
		<option value=2 selected> Semana </option>
		<option value=3> Mes </option>
		</select>";
	}
else if($_POST['time']==3){
	$Time.="<option value=0> Hora </option>
		<option value=1> Dia </option>
		<option value=2> Semana </option>
		<option value=3 selected> Mes </option>
		</select>";
	}

$indate=$conexion->select_fecha($_POST['fechaini'],0);
$endate=$conexion->select_fecha($_POST['fechater'],1);
$types=$conexion->select_type('sensor','tipo',$_POST['tipo']);
$tipoestadistica="<form action='estadisticas.php' method=post> Tipo estadistica $Time  Dato $types Inicio $indate Final $endate <input type=submit value='generar'></form>";
echo $tipoestadistica;
$pseud=array('fecha','00','01','02','03','04','05','06','07','08','09','10','11','12','13','14','15','16','17','18','19','20','21','22','23');
if(isset($_POST['time'])){
        $opc=$_POST['time'];
        if($opc==0){
                echo "ESTADISTICA POR HORA<br>La tabla muestra el promedio por hora de cada dia para el sensor ".$_POST['tipo']."<br><br>";
                $sistemas=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
                while($row=mysqli_fetch_array($sistemas)){
                        $sql="where fecha>='".$_POST['fechaini']."' and fecha<='".$_POST['fechater']."' and
                                tipo='".$_POST['tipo']."' and sistema=".$row['sistema'];
                        $tabla=$conexion->query_table_special($pseud,$pseud,'estadistica_hora',$sql);
                        $res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
                        $sis=mysqli_fetch_array($res);
                        echo $sis['nombre']."<br>".$tabla."<br>";
                        }
                }
	if($opc==1){
		echo "ESTADISTICA POR DIA<br>";
                $sistemas=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
                while($row=mysqli_fetch_array($sistemas)){
                        $sql="select distinct fecha from monitoreo_sensor where fecha>='".$_POST['fechaini']."' and fecha<='".$_POST['fechater']."'";
                        $result=$conexion->query($sql);
                        $tabla="<table border> <tr><th>DIA</th><th>PROMEDIO</th></tr>";
                        while($row2=mysqli_fetch_array($result)){
                                $tabla.="<tr><td>".$row2['fecha']."</td>";
                                $rprom=$conexion->query("select avg(valor) as promedio from monitoreo_sensor where id_sensor=(select id from sensor where 
                                                        tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha='".$row2['fecha']."'");
                                $riw=mysqli_fetch_array($rprom);
                                $prom=number_format($riw['promedio'],2);
                                $tabla.="<td>".$prom."</td></tr>";
                                }
                        $tabla.="</table>";
                        $res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
                        $sis=mysqli_fetch_array($res);
                        echo $sis['nombre']."<br>".$tabla;
                        }
		}
	if($opc==2){
		echo "ESTADISTICA POR SEMANA<br>";
                $sistemas=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
                while($row=mysqli_fetch_array($sistemas)){
                        $sql="select distinct fecha from monitoreo_sensor where fecha>='".$_POST['fechaini']."' and fecha<='".$_POST['fechater']."'";
                        $result=$conexion->query($sql);
                        $tabla="<table border> <tr><th>SEMANA</th><th>PROMEDIO</th></tr>";
			$query="select avg(valor) as promedio from monitoreo_sensor where id_sensor=(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha in (";
			$count=0;
                        while($row2=mysqli_fetch_array($result)){
				$count++;
				if($count % 7 ==1){$tabla.="<tr><td>".$row2['fecha'];}
				if($count % 7 !=0){
					$query.="'".$row2['fecha']."',";
					}
				else{
					$query.="'".$row2['fecha']."')";
					$tabla.=" hasta ".$row2['fecha']."</td>";
                                	$rprom=$conexion->query($query);
                                	$riw=mysqli_fetch_array($rprom);
                                	$prom=number_format($riw['promedio'],2);
                                	$tabla.="<td>".$prom."</td></tr>";
					$query="select avg(valor) as promedio from monitoreo_sensor where id_sensor=(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha in (";
					}
                                }
                        $tabla.="</table>";
                        $res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
                        $sis=mysqli_fetch_array($res);
                        echo $sis['nombre']."<br>".$tabla;
                        }
		}
	if($opc==3){
		echo "ESTADISTICA POR MES<br>";
                $sistemas=$conexion->query("select sistema from sensor where tipo='".$_POST['tipo']."'");
                while($row=mysqli_fetch_array($sistemas)){
                        $sql="select distinct fecha from monitoreo_sensor where fecha>='".$_POST['fechaini']."' and fecha<='".$_POST['fechater']."'";
                        $result=$conexion->query($sql);
                        $tabla="<table border> <tr><th>MES</th><th>PROMEDIO</th></tr>";
			$mes="";
                        while($row2=mysqli_fetch_array($result)){
				if($mes!=substr($row2['fecha'],5,2)){
					$mes=substr($row2['fecha'],5,2);
                                	$tabla.="<tr><td>".$conexion->makemes($mes)."</td>";
					$query="select avg(valor) as promedio from monitoreo_sensor where id_sensor=(select id from sensor where tipo='".$_POST['tipo']."' and sistema=".$row['sistema'].") and fecha like '_____".$mes."___'";
	                                $rprom=$conexion->query($query);
        	                        $riw=mysqli_fetch_array($rprom);
                	                $prom=number_format($riw['promedio'],2);
                        	        $tabla.="<td>".$prom."</td></tr>";
                                	}
				}
                        $tabla.="</table>";
                        $res=$conexion->query("select nombre from sistema where id=".$row['sistema']);
                        $sis=mysqli_fetch_array($res);
                        echo $sis['nombre']."<br>".$tabla;
                        }
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

