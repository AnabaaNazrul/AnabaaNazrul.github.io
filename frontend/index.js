console.log ("hello world")
fetch('http://192.168.43.23:5000/')
  .then(response => response.json())
  .then(data => {
      document.getElementById("content").innerHTML=data.data
      console.log (data)
  });

