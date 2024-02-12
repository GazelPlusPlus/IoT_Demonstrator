function printDashboard(windrowId) {
  var srcUrl = "";
  switch(windrowId) {
    case "windrow1system1device1":
      srcUrl = "http://135.125.75.21:3000/d/e6ae1bfa-4759-422a-88de-cd4cc9a679d2/windrow1-system1-device3?orgId=1&refresh=5s&from=1706730524446&to=1706816924446&theme=light";
      break;
    case "windrow1system1device2":
      srcUrl = "http://135.125.75.21:3000/d/ac8c7c23-3ceb-4483-a662-53e5c212ce13/windrow1-system1-device2?orgId=1&refresh=5s&from=1706723767741&to=1706810167741&theme=light";
      break;
    case "windrow1system1device3":
      srcUrl = "http://135.125.75.21:3000/d/e6ae1bfa-4759-422a-88de-cd4cc9a679d2/windrow1-system1-device3?orgId=1&refresh=5s&from=1706723853124&to=1706810253124&theme=light";
      break;
  }
  document.querySelector('.visualisation-message').style.display = 'none';
  document.getElementById("dashboard").src = srcUrl;
}