<!DOCTYPE html>
<html>
<head>
    <title>Gallery</title>
</head>
<body>
    <h1>Gallery</h1>
    <div id="imageGallery">
        <?php
        $dir = 'resources/bin';
        $files = scandir($dir);
        $ext = '.png';
        foreach ($files as $img) {
            if ( substr_compare($img, $ext, -strlen($ext), strlen($ext)) === 0 ) {
                ?>
                    <h2 style="font-family: monospace;"><?=$img?></h2>
                    <div style="text-align: center; margin-bottom: 100px;">
                        <img src="<?=$img?>">
                    </div>
                <?
            }
        }
        ?>
    </div>
</body>
</html>