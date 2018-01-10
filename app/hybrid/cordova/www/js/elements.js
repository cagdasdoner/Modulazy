
$('#submitCredentials').click(function() {
  startTransaction($("#ssid").val(), $("#pass").val());
  $('#status').text("Transmitting...");
});

function transferCompleted() {
  $('#status').text("Completed.");
}
