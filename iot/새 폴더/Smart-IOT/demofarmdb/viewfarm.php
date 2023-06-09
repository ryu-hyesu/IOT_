<scrippt language='javascript'>
  window.setTimeout('window.location.reload()',1000); </script>

<?php $connect = mysqli_connect("localhost", "root", "root")
   or die("Connect Fail: " . mysql_error());
echo "Connect Success!";
mysqli_select_db($connect, "demofarmdb") or die("Select DB Fail!");
$query = "select * from thl order by time desc limit 10";
$result = mysqli_query($connect, $query) or die("Query Fail: " . mysql_error());
echo "<table>\n";
echo "<tr><td>Time</td><td>TEM</td><td>HUM</td><td>ILL</td></tr>";

while ($line = mysqli_fetch_array($result, MYSQLI_ASSOC)) {
    echo "\t<tr>\n";
    foreach ($line as $col_value) {
        echo "\t\t<td>$col_value  </td>\n";
    }
    echo "\t</tr>\n";
}

echo "</table>\n"; mysqli_free_result($result); mysqli_close($connect); ?>

