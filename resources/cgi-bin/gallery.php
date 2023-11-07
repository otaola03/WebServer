<!DOCTYPE html>
<html>
<head>
    <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
    <meta content="utf-8" http-equiv="encoding">
    <title>Gallery</title>
    <style>
    .image-row {
        display: flex;
        flex-direction: row;
        justify-content: center;
    }

    .gallery-image-container {
        position: relative;
        margin: 10px;
        display: inline-block;
    }

    .gallery-image {
        width: 200px; /* Ancho deseado */
        height: auto; /* Altura ajustada automáticamente para mantener la proporción original */
        cursor: pointer; /* Cambia el cursor al pasar sobre la imagen */
    }

    .delete-icon {
        position: absolute;
        top: 0;
        right: 0;
        cursor: pointer;
        width: 20px; /* Ancho deseado del icono de eliminación */
        height: 20px; /* Altura deseada del icono de eliminación */
    }

    #imageModal {
        display: none;
        position: fixed;
        z-index: 1;
        padding-top: 100px;
        left: 0;
        top: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.9);
    }

    #modalContent {
        text-align: center;
    }

    #modalImage {
        max-width: 80%;
        max-height: 80%;
    }

    #closeModal {
        color: #fff;
        position: absolute;
        top: 10px;
        right: 20px;
        font-size: 24px;
        cursor: pointer;
    }
    </style>
</head>
<body>
    <h1>Gallery</h1>
    <div id="imageGallery">
        <?php
        $carpeta = "resources/bin"; // Ruta de la carpeta de imágenes
        $tamañoDeseado = 200; // Ancho deseado en píxeles

        // Escanea la carpeta y muestra todas las imágenes
        $archivos = scandir($carpeta);
        $imagenCount = 0;

        foreach ($archivos as $archivo) {
            if (in_array(pathinfo($archivo, PATHINFO_EXTENSION), array("jpg", "jpeg", "png", "gif"))) {
                if ($imagenCount % 4 == 0) {
                    echo "<div class='image-row'>";
                }

                $rutaImagen = $carpeta . $archivo;
                echo "<div class='gallery-image-container'>";
                echo "<img src='$archivo' class='gallery-image' onclick='mostrarImagen(this.src)' />";
                echo "<img src='delete.png' alt='Eliminar' class='delete-icon' onclick='eliminarImagen(this)' />";
                echo "</div>";

                $imagenCount++;

                if ($imagenCount % 4 == 0) {
                    echo "</div>";
                }
            }
        }
        ?>
    </div>
    <div id="imageModal">
        <div id="modalContent">
            <span id="closeModal" onclick="cerrarModal()">&times;</span>
            <img id="modalImage" src="">
        </div>
    </div>
    <script>
        function mostrarImagen(src) {
            var modal = document.getElementById("imageModal");
            var modalImage = document.getElementById("modalImage");

            modal.style.display = "block";
            modalImage.src = src;
        }

        function cerrarModal() {
            var modal = document.getElementById("imageModal");
            modal.style.display = "none";
        }

        async function eliminarImagen(deleteIcon) {
            try{
                var imageContainer = deleteIcon.parentElement;
                var image = imageContainer.querySelector(".gallery-image");
                var imagePath = image.src; // Obtener la ruta de la imagen
                const res = await fetch(imagePath, {
                method: 'DELETE',
                headers: {
                    'Content-Type': 'application/json'
                }
                })
            }
            catch(error){
                console.log(error);
            }
            location.reload();
        }
    </script>
</body>
</html>
