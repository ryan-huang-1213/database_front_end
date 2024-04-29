var An = true;
var Or = false;
function search() {
  var input = document.getElementById("searchInput").value;
  // Perform search using the input
  console.log("Searching for: " + input);
  // You'll need to implement actual search functionality as needed.
}

function OR() {
  Or = true;
  An = false;
  updateSearchCondition();
}

function AND() {
  An = true;
  Or = false;
  updateSearchCondition();
}

function updateSearchCondition() {
  var conditionText = An ? "AND" : "OR";
  document.getElementById("searchCondition").innerText =
    "Current Search Condition: " + conditionText;
}
