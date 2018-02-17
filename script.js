var nbind = require('nbind');
var lib = nbind.init().lib;

dm = lib.GRDevManager();
con = lib.GRConnection();
msg = lib.GRMessage();
alg_msg = lib.GRAlgMessage();


//m = lib.GRMadgwick(); // no problems


// see show_undefined_symbols, GRT::DTW is undefined. 
//alg = lib.GRAlgorithm();
//b = lib.GRGrt(); // here if a problem





// console.log(dm.getAvalibleDevices());
available_devs = dm.getAvalibleDevices();
console.log(available_devs);
console.log("Got devices, printing names:");
available_devs.forEach(function(el) {
    console.log(el.get_name());
});
/*
if (available_devs.length < 1) return;

con = dm.setActiveDevice(available_devs[0].get_id());
con.connectSocket();

alg.setupMadgwick(140, 140, 140, 140, 140, 220); //need to check

let i = 0;
let thumbImu;
while(true)
{
  conn.getData(msg);
  alg.madgwickUpdate(msg, alg_msg, 1, "flag");
  
  thumbImu = msg.get_thumb();
  console.log(thumbImu.get_acc());
  
  msg.clear();
  alg_msg.clear();
  i++;
}
*/
