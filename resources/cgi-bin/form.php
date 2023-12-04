<!DOCTYPE html>
<html>
<body>

<h2>Formulario procesado con PHP</h2>

<?php
	$arguments = $_SERVER['argv'];
	$queryString = $arguments[1];

	// Analiza la cadena de consulta
	parse_str($queryString, $queryParams);
	
	// Obtiene los valores de nombre y apellido
	$firstName = isset($queryParams['fname']) ? $queryParams['fname'] : '';
	$lastName = isset($queryParams['lname']) ? $queryParams['lname'] : '';

    // Mostrar los valores
    echo "Nombre: " . $firstName . "<br>";
    echo "Apellido: " . $lastName . "<br>";
?>

</body>
</html>
