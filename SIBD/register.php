<!DOCTYPE html>
<html>
  <head>
		<meta charset="utf-8">
    <title>Register Page</title>
  </head>

  <body>
    <header>

			<h1>Register</h1>

		</header>

    <section>

			<div>

				<form action="character_creation.php" method = "post">

          <input type = "text" name = "firstname" required>
          <input type = "text" name = "lastname" required>
          <input type = "text" name = "password" required>
          <input type = "text" name = "email" required>
          <input type = "text" name = "country" required>

					<label for="male">Male</label>
					<input type="radio" name="gender" id="male" value="male"><br>
					<label for="female">Female</label>
					<input type="radio" name="gender" id="female" value="female"><br>

          <button type= "submit">Register</button>
  		  </form>
			</div>


		</section>
  </body>
</html>
