<!DOCTYPE html>
<html>
  <head>
		<meta charset="utf-8">
    <title>Character Creation</title>
  </head>

  <body>
    <header>
			<h1>Character Creation</h1>
		</header>
    <section>

      <?php
        echo $_POST["firstname"];
      ?>
			<div>
				<form action="main_page.php" method = "post">
        	<label>Race</label>
          <select name="race" method = "post">
            <option value="Human">Human</option>
            <option value="Dwarf">Dwarf</option>
            <option value="Elf">Elf</option>
          </select>

        	<label>Class</label>
          <select name="class">
            <option value="Knight">Knight</option>
            <option value="Wizard">Wizard</option>
          </select>
        </form>

        <input type = "text" name = "charname" required>

        <button type= "submit">Create</button>

			</div>
			</section>
  </body>
</html>
