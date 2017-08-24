<?php
  include_once('config/init.php');
?>

<!DOCTYPE html>
<html>
  <head>
		<meta charset="utf-8">
    <title>SNWK 2</title>
  </head>

  <body>
    <header>
			<!--<h1>Super Neon Wizard Knights II</h1>-->
      <img src = "SNWK.png" alt="SNWK Logo" width = "500" height = "500">
		</header>
    <section>
			<div>
				<form action="login.php" method = "post">
					<div class="container">
						<label><b>Username</b></label>
						<input type="text" placeholder="Enter Username" name="uname" required>

						<label><b>Password</b></label>
						<input type="password" placeholder="Enter Password" name="psw" required>

					</div>
				</div>
				<button type= "submit">Login</button>
			</form>

			<form action = "register.php">
				<input type= "submit"  value="Register">
			</form>

			</div>
			</section>
  </body>
</html>
