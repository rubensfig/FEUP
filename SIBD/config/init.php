<?php
  session_start();

  error_reporting(error_reporting() & ~E_NOTICE);

  $conn = new PDO('mysql:host=dbm.fe.up.pt;port=5432;dbname=FinalProject', 'sibd1602', 'palavrapasse');
?>
