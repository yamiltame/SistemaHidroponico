<?php
class mySQL{
	var $host;
	var $user;
	var $pass;
	var $base;
	public $dbc;

	public function makemes($mes){
		if($mes==01){$r="Enero";}
		if($mes==02){$r="Febrero";}
		if($mes==03){$r="Marzo";}
		if($mes==04){$r="Abril";}
		if($mes==05){$r="Mayo";}
		if($mes==06){$r="Junio";}
		if($mes==07){$r="Julio";}
		if($mes==08){$r="Agosto";}
		if($mes==09){$r="Septiembre";}
		if($mes==10){$r="Octubre";}
		if($mes==11){$r="Noviembre";}
		if($mes==12){$r="Diciembre";}
		return $r;
		}

	public function conectar(){
		$this->host='localhost';
		$this->user='root';
		$this->pass='qwerty';
		$this->base='betha';
		$this->dbc=mysqli_connect($this->host,$this->user,$this->pass,$this->base) or die('Error en la  conetzion \n');
		$acentos=$this->query("SET NAMES 'utf8'");
		}

	public function query($sql){
		$result=mysqli_query($this->dbc,$sql) or die('error en la consulta'.mysql_error());
		return $result;
		}

        public function query_count($table,$condition){
                $sql="select count(*) as numr  from ".$table." ".$condition;
                $result =  $this->query($sql);
                $row=mysqli_fetch_array($result);
                return $row['numr'];
	        }

        public function query_table_special($campos,$pseudonimos,$nombre,$condicion){
                $tabla="<table border><tr>";
		$sql="select ";
                $max=sizeof($campos);
                for($i=0;$i<$max;$i++){
                        $tabla.="<th>".$pseudonimos[$i]."</th>";
                        $sql.=$nombre.".".$campos[$i].",";
                        }
		$tabla.="</tr>";
                $sql.="'' from ".$nombre." ".$condicion;
                $result=$this->query($sql);
                while($row=mysqli_fetch_array($result)){
                        $tabla.="<tr>";
                        foreach($campos as $c){
                                $tabla.="<td>".number_format($row[$c],1,".","")."</td>";
                                }
                        $tabla.="</tr>";
                        }
                $tabla.="</table>";
                return $tabla;
                }

        public function query_table($campos,$pseudonimos,$nombre,$condicion){
                $tabla="<table border><tr>";
                $sql="select ";
                $tabla.="</tr>";
                $max=sizeof($campos);
                for($i=0;$i<$max;$i++){
                        $tabla.="<th>".$pseudonimos[$i]."</th>";
                        $sql.=$campos[$i].",";
                        }
                $sql.="'' from ".$nombre." ".$condicion;
//		echo $sql."<br>";
                $result=$this->query($sql);
                while($row=mysqli_fetch_array($result)){
                        $tabla.="<tr>";
                        for($i=0;$i<$max;$i++){
                                $tabla.="<td>".$row[$i]."</td>";
                                }
                        $tabla.="</tr>";
                        }
                $tabla.="</table><br>";
                return $tabla;
                }

	public function make_link($file,$text,$opc){
		$link="<form action='".$file."' method=post><input type=hidden name=opcion value='".$opc."'><input type=submit value='".$text."'></form>";
		return $link;
		}

	public function escoger_sistema($file,$parent){
                $form="<form action=\"$file\" method=\"post\"><select name=\"sistema\">";
                $consulta="select nombre,id from sistema where parent=".$parent;
                $result=$this->query($consulta);
                while($row=mysqli_fetch_array($result)){
                        $form.='<option value="'.$row['id'].'">'.$row['nombre']."</option>";}
                $form.="</Select><input type=\"hidden\" name=\"parent\" value=$parent>";
		if($parent==0){$form.="<input type=submit value=\"Editar sistema\">";}
		else{ $form.="<input type=submit value='Editar Subsistema'>";}
		$form.="</form>";
                return $form;
                }

	public function all_systems($default){
                $form="<select name=\"sistema\">";
                $consulta="select nombre,id from sistema";
                $result=$this->query($consulta);
                while($row=mysqli_fetch_array($result)){
                        if($row['id']==$default){$form.="<option value='".$row['id']."' selected>".$row['nombre']."</option>";}
                        else{$form.='<option value="'.$row['id'].'">'.$row['nombre']."</option>";}
			}
                $form.="</Select>";
                return $form;
                }


	public function select_fecha($default,$initfin){
		if($initfin==0){
			$sql="select distinct fecha from monitoreo_sensor order by id_monitoreo";
			$form="<select name='fechaini'>";
			}
		else{
			$sql="select distinct fecha from monitoreo_sensor order by id_monitoreo desc";
			$form="<select name='fechater'>";
			}
		$res=$this->query($sql);
		while($row=mysqli_fetch_array($res)){
			if($row['fecha']==$default){$form.="<option value='".$row['fecha']."' selected>".$row['fecha']."</option>";}
			else{$form.="<option value='".$row['fecha']."'>".$row['fecha']."</option>";}
			}
		$form.="</select>";
		return $form;
		}

	public function select_hora($default,$initfin){
		if($initfin==0){ $form="<select name='horaini'>";
			for($i=0;$i<24;$i++){
				if($i==$default){$form.="<option value='".$i."' selected>".$i."</option>";}
				else{$form.="<option value='".$i."'>".$i."</option>";}
				}
			}
		else{$form="<select name='horafin'>";
			for($i=23;$i>-1;$i--){
				if($i==$default){$form.="<option value='".$i."' selected>".$i."</option>";}
				else{$form.="<option value='".$i."'>".$i."</option>";}
				}
			}
		$form.="</select>";
		return $form;
		}

	public function select_riego($default){
                $form="<select name=\"riego\">";
                $consulta="select id,tipo from riego";
                $result=$this->query($consulta);
                while($row=mysqli_fetch_array($result)){
			if($row['id']==$default){$form.='<option value="'.$row['id'].'" selected>'.$row['tipo']."</option>";}
                        else{$form.='<option value="'.$row['id'].'">'.$row['tipo']."</option>";}
			}
                $form.="</Select>";
                return $form;
                }

	public function escoger_plan($default){
                $form="<select name=\"planificador\">";
                $consulta="select cultivo,id from planificador";
                $result=$this->query($consulta);
                while($row=mysqli_fetch_array($result)){
			if($row['id']==$default){$form.='<option value="'.$row['id'].'" selected>'.$row['cultivo']."</option>";}
                        else{$form.='<option value="'.$row['id'].'">'.$row['cultivo']."</option>";}
			}
                $form.="</Select>";
                return $form;
                }

	public function escoger_padre($default){
                $form="<select name=\"padre\">";
                $consulta="select nombre,id from sistema where parent=0";
                $result=$this->query($consulta);
                while($row=mysqli_fetch_array($result)){
			if($row['id']==$default){$form.='<option value="'.$row['id'].'" selected>'.$row['nombre']."</option>";}
                        else{$form.='<option value="'.$row['id'].'">'.$row['nombre']."</option>";}
			}
                $form.="</Select>";
                return $form;
                }

	public function new_subsys(){
		$planers=$this->escoger_plan(0);
		$systems=$this->escoger_padre(0);
		$formulario_subsys="<form action='sistema.php' method='post'>
			Nombre:<br><input type='text' name='nombre'><br>ubicacion:<br><input type='text' name='ubicacion'><br>URL:<br><input type='text' name='url'>
			<br>Planificador:<br>$planers <br>Sistema Padre: <br>$systems <br>Estado:<br><select name='estado'><option 
			value=1>On</option>;<option value=0>Off</option>;</select> <br><input type='submit' value='crear'><input type=hidden name=nuevo value=1></form>";
		return $formulario_subsys;
		}

	public function new_sys(){
		$planers=$this->escoger_plan(0);
		$formulario_sys="<form action='sistema.php' method='post'>Nombre:<br><input type='text' name='nombre'><br>ubicacion:<br><input type='text' 
			name='ubicacion'><br>URL:<br><input type='text' name='url'><br>Planificador:<br>$planers <br>Estado:<br><select name='estado'><option 
			value=1>On</option>;<option value=0>Off</option>;</select> <br><input type='submit' value='crear'><input type='hidden' name='padre' value=0>
			<input type=hidden name=nuevo value=1></form>";
		return $formulario_sys;
		}
	public function subsys_form($nombre,$ubicacion,$url,$planificador,$padre,$id){
		$planers=$this->escoger_plan($planificador);
		$systems=$this->escoger_padre($padre);
		$formulario_subsys="<form action='sistema.php' method='post'>
			Nombre:<br><input type='text' name='nombre' value=$nombre><br>ubicacion:<br><input type='text' name='ubicacion' 
			value=$ubicacion><br>URL:<br><input type='text' 
			name='url' value=$url><br>Planificador:<br>$planers <br>Sistema Padre: <br>$systems <br>Estado:<br><select name='estado'><option 
			value=1>On</option>;<option value=0>Off</option>;</select> <br><input type='submit' value='editar info'>
			<input type='hidden' name='id' value=$id><input type=hidden name=edit value=1></form>";
		return $formulario_subsys;
		}

	public function system_form($nombre,$ubicacion,$url,$planificador,$id){
		$planers=$this->escoger_plan($planificador);
		$formulario_sys="<form action='sistema.php' method='post'>Nombre:<br><input type='text' name='nombre' value=$nombre><br>ubicacion:<br><input type='text' 
name='ubicacion' value=$ubicacion><br>URL:<br><input type='text' name='url' value=$url><br>Planificador:<br>$planers <br>Estado:<br><select name='estado'><option 
value=1>On</option>;<option value=0>Off</option>;</select> <br><input type='submit' value='editar info'><input type='hidden' name='padre' value=0>
		<input type='hidden' name='id' value=$id><input type=hidden name=edit value=1></form>";
		return $formulario_sys;
		}
	
	public function select_type($table,$column_name,$default){
		$form="<select name=\"$column_name\">";
		$sql="SELECT COLUMN_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '$table' AND COLUMN_NAME = '$column_name'";
		$result = $this->query($sql);
		$row = mysqli_fetch_array($result);
		$enumList = explode(",", str_replace("'", "", substr($row['COLUMN_TYPE'], 5, (strlen($row['COLUMN_TYPE'])-6))));
		foreach($enumList as $value){
			if($value==$default){ $form.="<option value=\"$value\" selected>$value</option>";}
	    		else{ $form.="<option value=\"$value\">$value</option>";}
			}
		$form.="</select>";
		return $form;
		}

	public function edit_riego($tipo,$t_nor,$p_nor,$Te_nor,$t_mas,$p_mas,$Te_mas,$t_men,$p_men,$Te_men,$id){
		$formulario="Riego $tipo <br><form action='riego.php' method='post'><input type='hidden' name=id value=$id>
			tipo:<br><input type='text' name='tipo' value=$tipo><br>
			Tiempo_normal:<br><input type='float' name='t_nor' value=$t_nor><br>
			Periodo_normal:<br><input type='float' name='p_nor' value=$p_nor><br>
			Temperatura_normal:<br><input type='float' name='Te_nor' value=$Te_nor><br>
			Tiempo_mas:<br><input type='float' name='t_mas' value=$t_mas><br>
			Periodo_mas:<br><input type='float' name='p_mas' value=$p_mas><br>
			Temperatura_mas:<br><input type='float' name='Te_mas' value=$Te_mas><br>
			Tiempo_menos:<br><input type='float' name='t_men' value=$t_men><br>
			Periodo_menos:<br><input type='float' name='p_men' value=$p_men><br>
			Temperatura_menos:<br><input type='float' name='Te_men' value=$Te_men><br>
			<input type='hidden' name='edit' value=1>
			<input type='submit' value='editar'></form>";
		return $formulario;
		}

	public function new_riego(){
		$formulario="Riego Nuevo <br><form action='riego.php' method='post'>
			tipo:<br><input type='text' name='tipo'><br>
			Tiempo_normal:<br><input type='float' name='t_nor'><br>
			Periodo_normal:<br><input type='float' name='p_nor'><br>
			Temperatura_normal:<br><input type='float' name='Te_nor'><br>
			Tiempo_mas:<br><input type='float' name='t_mas'><br>
			Periodo_mas:<br><input type='float' name='p_mas'><br>
			Temperatura_mas:<br><input type='float' name='Te_mas'><br>
			Tiempo_menos:<br><input type='float' name='t_men'><br>
			Periodo_menos:<br><input type='float' name='p_men'><br>
			Temperatura_menos:<br><input type='float' name='Te_men'><br>
			<input type='hidden' name='nuevo' value=1>
			<input type='submit' value='registrar'></form>";
		return $formulario;
		}

	public function edit_plan($riego,$cultivo,$phmax,$phmin,$hmax,$hmin,$tem,$ran,$id){
		$riegos=$this->select_riego($riego);
		$formulario="Planificador de $cultivo <br><form action='planificador.php' method='post'><input type='hidden' name=id value=$id>riego:<br>$riegos 
			<br>Cultivo:<br><input type='text' name='cultivo' value=$cultivo><br>
			phmax:<br><input type='float' name='phmax' value=$phmax><br>
			phmin:<br><input type='float' name='phmin' value=$phmin><br>
			hmax:<br><input type='float' name='hmax' value=$hmax><br>
			hmin:<br><input type='float' name='hmin' value=$hmin><br>
			temperatura ambiente:<br><input type='float' name='temp' value=$tem><br>
			rango ambiente:<br><input type='float' name='rango' value=$ran><br>
			<input type='hidden' name='edit' value=1>
			<input type='submit' value='editar'></form>";
		return $formulario;
		}
	public function new_plan(){
		$riegos=$this->select_riego(0);
		$formulario="Planificador Nuevo <br><form action='planificador.php' method='post'><input type='hidden' name=nuevo value=1>riego:<br>$riegos 
			<br>Cultivo:<br><input type='text' name='cultivo'><br>
			phmax:<br><input type='float' name='phmax'><br>
			phmin:<br><input type='float' name='phmin'><br>
			hmax:<br><input type='float' name='hmax'><br>
			hmin:<br><input type='float' name='hmin'><br>
			temperatura ambiente:<br><input type='float' name='temp'><br>
			rango ambiente:<br><input type='float' name='rango'><br>
			<input type='submit' value='registrar'></form>";
		return $formulario;
		}

	public function edit_form($pin,$tipo,$muestreo,$sistema,$id,$sensoract){
		if($sensoract==1){
			$formulario="Sensor de $tipo <br><form action='editarsensores.php' method='post'><input type='hidden' name=sistema value=$sistema><input type='hidden' name=id value=$id>pin:<br><input type='integer' name=pin value=$pin><br>muestreo en min:<br><input type='integer' name='muestreo' value=$muestreo><br><input type='submit' value='editar'></form>";
			}
		if($sensoract==2){
			$formulario="Actuador $tipo <br><form action='editaractuadores.php' method='post'>pin:<br><input type='integer' name=pin value=$pin><br><input type='hidden' name=sistema value=$sistema><input type='hidden' name=id value=$id><input type='submit' value='editar'></form>";
			}
		return $formulario;
		}

	public function new_form($sistema,$sensoract){
		if($sensoract==1){
			$types=$this->select_type("sensor","tipo",$tipo);
			$formulario="Nuevo Sensor <br><form action='editarsensores.php' method='post'>pin:<br><input type='integer' name=pin><br>tipo: <br>$types <br>muestreo en min:<br><input type='integer' name='muestreo'><br><input type='hidden' name=sistema value=$sistema><input type='hidden' name=nuevo value='new'><input type='submit' value='crear'><br></form>";
			}
		if($sensoract==2){
			$types=$this->select_type("actuador","tipo",$tipo);
			$formulario="Nuevo Actuador <br><form action='editaractuadores.php' method='post'>pin:<br><input type='integer' name=pin><br>tipo: <br>$types <br><input type='hidden' name=sistema value=$sistema><input type='hidden' name=nuevo value='new'><input type='submit' value='crear'><br></form>";
			}
		return $formulario;
		}

	public function borrar_sys($id){
		//obtener hijos
		$sql="select id,nombre from sistema where parent=".$id;
		$result=$this->query($sql);
		while($row=mysqli_fetch_array($result)){
			$cmd="./kill ".$row['id']." ".$row['nombre'];
			system($cmd);
			$this->query("delete from sensor where sistema=".$row['id']);
			$this->query("delete from actuador where sistema=".$row['id']);
			$this->query("delete from sistema where id=".$row['id']);
			}
		$this->query("delete from sensor where sistema=".$id);
		$this->query("delete from actuador where sistema=".$id);
		$this->query("delete from sistema where id=".$id);
		}

/*	public function query_form($area,$file,$matricula){
		$name=$this->getarea($area);
		$form= "<form action=\"$file\" method=\"post\">".$name."<select name=\"curso\">";
		$sql="select clave,Name from materias where clave like '".$area."%' order by Name";
		$result=$this->query($sql);
		while($row=mysqli_fetch_array($result)){
			$form.='<option value="'.$row['clave'].'">'.$row['Name']."</option>";}
		$form.="</select><input type=\"hidden\" name=\"matricula\" value=$matricula><input type=submit value=\"Seleccionar\"></form>";
		return $form;
		}


	public function delete_form($matricula,$file,$table,$type){
		$form="<form action=\"$file\" method=\"post\"><select name=\"Eliminar\">";
		$consulta="select clave,Name from materias where clave in (select clave from ".$table." where ".$type."='".$matricula."') order by Name";
		$result=$this->query($consulta);
		while($row=mysqli_fetch_array($result)){
			$form.='<option value="'.$row['clave'].'">'.$row['Name']."</option>";}
		$form.="</Select><input type=\"hidden\" name=\"matricula\" value=$matricula><input type=submit value=\"Eliminar\"></form>";
		return $form;
		}

	public function getarea($area){
		$field="";
		if($area=="TC"){ $field="Tronco comun";}
		if($area=="E"){ $field="Matematicas educativas";}
		if($area=="M"){ $field="Matematicas";}
		if($area=="F"){ $field="Fisica";}
		if($area=="C"){ $field="Computacion";}
		return $field;
		}


	public function make_schedule($tabla,$matricula,$tipo){
		$semana = array("lunes", "martes", "miercoles", "jueves", "viernes");
		foreach($semana as $dia){
			$sql="insert into ".$tabla." (".$tipo.",dia) values('".$matricula."','".$dia."')";
			$this->query($sql);
		}
	}
	public function make_schedule_materia($clave,$demanda){
		$deletetable="delete from horariosmaterias where clave='".$clave."'";
		$this->query($deletetable);
		$condicion="where clave='".$clave."'";
		$abierta=$this->query_count("horariosmaterias",$condicion);
		if($abierta==0){
			if($demanda>=3){
				$semana = array("lunes", "martes", "miercoles", "jueves", "viernes");
				foreach($semana as $dia){
					$sql="insert into horariosmaterias (clave,dia) values('".$clave."','".$dia."')";
					$this->query($sql);
				}
			}
			else{
				$sql="select Name from materias where clave='".$clave."'";
				$consulta=$this->query($sql);
				$nombre=mysqli_fetch_array($consulta);
				$faltantes=3-$demanda;
				echo "Para abrir ".$nombre['Name']." se necesitan ".$faltantes." estudiantes<br>";
			}
		}
	}

	public function disponibilidadmaestros($clave){
		$horasMaestro=array();
		$Maestros=array();
		$sql="select dia,horario from horariosmaterias where clave='".$clave."'";
		$consulta=$this->query($sql);
		while($materia=mysqli_fetch_array($consulta)){
			$dia=$materia['dia'];
			$horariomateria=str_split($materia['horario']);
			$mas="select numempleado,horasdisponibles from horariosprofesores where numempleado in (select numempleado from ofertas where clave='".$clave."') and dia='".$dia."'";
			$consult=$this->query($mas);
			while($profesor=mysqli_fetch_array($consult)){
				$numempleado=$profesor['numempleado'];
				$Maestros[$numempleado][$dia]=0;
				$horarioprofesor=str_split($profesor['horasdisponibles']);
				for($hr=7;$hr<20;$hr++){
					if($horariomateria[$hr-7]=="1" && $horarioprofesor[$hr-7]=="1"){ $Maestros[$numempleado][$dia]++;}
				}
			}
		}
	}

	public function horariosprovicionales(){
		$sql="delete from horariosprovicionales";
		$this->query($sql);
		$sql="select * from horariosalumnos";
		$consulta=$this->query($sql);
		while($alumno=mysqli_fetch_array($consulta)){
			$sql="insert into horariosprovicionales values('".$alumno['matricula']."','".$alumno['dia']."','".$alumno['horasdisponibles']."')";
			$this->query($sql);
		}
	}

	public function sincronizarmaestroalumnos($clave){
	        $sql="select numempleado,horasdisponibles from horariosprofesores where numempleado in (select numempleado from ofertas where clave='".$clave."')";
	        $consult=$this->query($sql);
	        while($profe=mysqli_fetch_array($consult)){
			$horasprofe=str_split($profe['horasdisponibles']);
			$sql="select * from horariosprovicionales where matricula in (select matricula from solicitudes where clave='".$clave."')";
			$consulta=$this->query($sql);
			while($alumno=mysqli_fetch_array($consulta)){
				$nuevo="";
				$horasalumno=str_split($alumno['horasdisponibles']);
				for($hr=7;$hr<20;$hr++){
					if($horasprofe[$hr-7]=="0"){ $horasalumno[$hr-7]="0";}
					$nuevo.=$horasalumno[$hr-7];
				}
				$sql="update horariosprovicionales set horasdisponibles='".$nuevo."' where matricula='".$alumno['matricula']."' and dia='".$alumno['dia']."'";
				$this->query($sql);
			}
		}
	}

	public function actualizarhora($hora,$dia,$clave){
		$sql="select matricula,horasdisponibles from horariosprovicionales where matricula in (select matricula from solicitudes where clave='".$clave."') and dia='".$dia."'";
		$consulta=$this->query($sql);
		while($alumno=mysqli_fetch_array($consulta)){
			$horas=str_split($alumno['horasdisponibles']);
			$horas[$hora-7]="0";
			$nuevo="";
			foreach($horas as $hr){
				$nuevo.=$hr;
			}
			$sql="update horariosprovicionales set horasdisponibles='".$nuevo."' where matricula='".$alumno['matricula']."' and dia='".$dia."'";
			$this->query($sql);
		}
	}

	public function formulario_horario($file,$matricula){
		$form="<style> table,th,td{border: solid black; border-collapse: collapse;}</style>";
		$form.="<form action=\"$file\" method=\"post\"><table><tr><th></th>";
		$semana=array("Lunes","Martes","Miercoles","Jueves","Viernes");
		foreach($semana as $dia){
			$form.="<th>".$dia."</th>";
		}
		$form.="</tr>";
		for($hora=7;$hora<20;$hora++){
			$form.="<tr><td>".$hora."</td>";
			foreach($semana as $dia){
				$form.="<td><input type=\"checkbox\" name=\"".$hora.$dia."\" value=\"0\"></td>";
			}
			$form.="</td>";
		}
		$form.="</table><input type=\"hidden\" name=\"matricula\" value=$matricula><input type=\"submit\" value=\"establecer disponibilidad\"></form>";
		return $form;
	}

	public function contar_indispuestos($materia){
		$semana=array("lunes","martes","miercoles","jueves","viernes");
		$unavaliable=array();
		foreach($semana as $dia){
		        $unavaliable[$dia]=array();
        		$sql="select matricula,horasdisponibles from horariosprovicionales where matricula in (select matricula from solicitudes where clave='".$materia."') and dia='".$dia."'";
        		$result=$this->query($sql);
        		while($horario=mysqli_fetch_array($result)){
        		        $horas[$horario['matricula']]=$horario['horasdisponibles'];
        		}
        		for($hr=7;$hr<20;$hr++){
        		        $unavaliable[$dia][$hr]=0;
        		        foreach($horas as $disp){
        		                $horasdisp=str_split($disp);
        		                if($horasdisp[($hr-7)]=="0"){
        		                        $unavaliable[$dia][$hr]+=1;
                        		}
                		}
        		}
		}
		return $unavaliable;
	}

	public function horasmateria($clave){
	        $sql="select HorasTeoria+HorasPractica as horas from materias where clave='".$clave."'";
	        $horas_necesarias=$this->query($sql); //consulta de las horas de la materia
	        $horasmateria=mysqli_fetch_array($horas_necesarias);
		return $horasmateria['horas'];
	}

	public function gethorario($clave,$dia){
		$sql="select horario from horariosmaterias where clave='".$clave."' and dia='".$dia."'";
		$consulta=$this->query($sql);
		$row=mysqli_fetch_array($consulta);
		return $row['horario'];
	}

	public function asignarhora($hora,$dia,$clave){
		$horario=$this->gethorario($clave,$dia);
		$horasmateria=str_split($horario);
		$horasmateria[$hora-7]="1";
		$nuevo="";
		foreach($horasmateria as $hr){
			$nuevo.=$hr;
		}
                $sql="update horariosmaterias set horario='".$nuevo."' where clave='".$clave."' and dia='".$dia."'";
                $this->query($sql);
	}*/


}
?>
