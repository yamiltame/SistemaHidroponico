<?php
require_once('conexion.php');
$conexion = new mySQL();
$conexion->conectar();

echo "SISTEMA DE HIDROPONIA <br><br>";


echo "<a href=\"sistema.php\">Sistemas</a> <br>";
echo "<a href=\"planificador.php\">Planificaciones</a> <br>";
echo "<a href=\"riego.php\">Riegos</a> <br>";
echo "<a href=\"estadisticas.php\">Estadisticas</a><br>";
echo "<a href=\"monitoreo.php\">Monitoreo</a>";

?>
