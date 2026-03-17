const char* htmlPageConfig PROGMEM = R"rawliteral(
  <div id="container1" class="container active">
    <h1>Bienvenue</h1>
    <p>Sur votre nouvelle équipement ModuleTIC. Nous allons configurer pas à pas votre équipement.</p>
    <button class="button" onclick="next(2)">Suivant</button>
  </div>

  <div id="container2" class="container">
    <h1>Configuration Wi-Fi</h1>
    <p>Souhaitez-vous connecter votre ModuleTIC sur un point d'accès Wi-Fi de votre domicile ?</p>
    <button class="button" onclick="handleRequest('wif1',function(){next(3);})">Oui</button>
    <button class="button secondary" onclick="handleRequest('wif0',function(){next(8);})">Non</button>
  </div>

  <div id="container3" class="container">
    <h1>Configuration Wi-Fi</h1>
    <div id="wifiList">
    </div>
    <button class="button warning" onclick="next(4)">Saisie manuelle</button>
  </div>

  <div id="container4" class="container">
    <h1>Configuration Wi-Fi</h1>
    <input type="text" id="ssid1" placeholder="Entrez le SSID">
    <button class="button" id="ssid1-submit">Soumettre</button>
  </div>

  <div id="container5" class="container">
    <h1>Configuration Wi-Fi</h1>
    <input type="password" id="mdp1" placeholder="Entrez le mot de passe">
    <input type="password" id="mdp11" placeholder="Confirmez le mot de passe">
    <button class="button" id="mdp1-submit">Soumettre</button>
  </div>

  <div id="container6" class="container">
    <h1>Configuration Wi-Fi</h1>
    <input type="text" id="hot1" placeholder="Entrez l'Hostname">
    <button class="button" id="hot1-submit">Soumettre</button>
  </div>

  <div id="container7" class="container">
    <h1>Configuration Wi-Fi</h1>
    <button class="button secondary" onclick="toggleAdvancedConfig()">Configurations avancées</button>
    <div id="advancedConfig" style="display: none;">
      <input type="text" id="ip" placeholder="Adresse IP de l'équipement">
      <input type="text" id="mask" placeholder="Masque de sous-réseau">
      <input type="text" id="gateway" placeholder="Passerelle">
    </div>
    <button class="button" id="advanced-submit">Soumettre</button>
  </div>

  <div id="container8" class="container">
    <h1>Configuration Wi-Fi</h1>
    <p>Votre ModuleTIC restera accessible en Wi-Fi sur ces identifiants. Souhaitez-vous personnaliser les identifiants Wi-Fi (SSID/Clé de sécurité réseau)?</p>
    <p class="alert">Il est fortement recommandé de modifier ces paramètres !</p>
    <button class="button" onclick="next(9)">Oui</button>
    <button class="button secondary" onclick="next(11)">Non</button>
  </div>

  <div id="container9" class="container">
    <h1>Configuration Wi-Fi</h1>
    <input type="text" id="ssid2" placeholder="Entrez le SSID">
    <button class="button" id="ssid2-submit">Soumettre</button>
  </div>

  <div id="container10" class="container">
    <h1>Configuration Wi-Fi</h1>
    <input type="password" id="mdp2" placeholder="Entrez le mot de passe">
    <input type="password" id="mdp22" placeholder="Confirmez le mot de passe">
    <button class="button" id="mdp2-submit">Soumettre</button>
  </div>

  <div id="container11" class="container">
    <h1>Configuration Web</h1>
    <p>Le port de service Web par défaut est le "80", souhaitez-vous le modifer ?</p>
    <button class="button" onclick="next(12)">Oui</button>
    <button class="button secondary" onclick="next(13)">Non</button>
  </div>

  <div id="container12" class="container">
    <h1>Configuration Web</h1>
    <input type="text" id="por1" placeholder="Entrez le port de service Web">
    <button class="button" id="por1-submit">Soumettre</button>
  </div>

  <div id="container13" class="container">
    <h1>Configuration WebSocket</h1>
    <p>Le port de service WebSocket par défaut est le "81", souhaitez-vous le modifer ?</p>
    <button class="button" onclick="next(14)">Oui</button>
    <button class="button secondary" onclick="next(20)">Non</button>
  </div>

  <div id="container14" class="container">
    <h1>Configuration WebSocket</h1>
    <input type="text" id="por2" placeholder="Entrez le port de service WebSocket">
    <button class="button" id="por2-submit">Soumettre</button>
  </div>

  <div id="container20" class="container">
    <h1>Félicitation</h1>
    <p>Vous avez terminé la configuration de votre équipement ModuleTIC. Votre équipement doit redémarrer...</p>
    <button class="button" onclick="handleRequest('end',function(){next(31);})">Redémarrer</button>
  </div>

  <div id="container31" class="container">
    <h1>Redémarrage</h1>
    <p>En cours...</p>
  </div>
)rawliteral";

const char* scriptsPageConfig PROGMEM = R"rawliteral(
  document.querySelectorAll('button[data-action]').forEach(function(button) {
    button.onclick = function() {
      var action = button.getAttribute('data-action');
      sendPostRequest('/' + action, 'data=' + action);
    };
  });

  function validateField(id,value) {
    if (id.startsWith('ssid')) {
      return validateSSID(value);
    } 
    if (id.startsWith('hot1')) {
      return validateHostname(value);
    } 
    if (id.startsWith('mdp')) {
      return validatePassword(id, value);
    }
    if (id.startsWith('por')) {
      return validatePORT(value);
    } 
    return true;
  }

  function validatePORT(value) {
    if (value < 1 || value > 65535) {
      alert("Le port est invalide. Veuillez entrer un numéro entre 1 et 65535.");
      return false;
    }
    return true;
  }
  
  function validateSSID(value) {
    if (value.length < 5 || value.length > 33) {
      alert("Le SSID doit contenir entre 5 et 33 caractères.");
      return false;
    }
    return true;
  }
  
  function validateHostname(value) {
    if (value.length === 0) {
      return true;
    } 
    if (value.length < 5 || value.length > 65) {
      alert("Le nom d'hôte doit contenir entre 5 et 65 caractères.");
      return false;
    }
    return true;
  }
  
  function validatePassword(id, value) {
    const confirmId = (id === 'mdp1') ? 'mdp11' : (id === 'mdp2') ? 'mdp22' : null;
    if (confirmId) {
      const confirmValue = document.getElementById(confirmId).value;
      if (value !== confirmValue) {
        alert("Les mots de passe ne correspondent pas. Veuillez réessayer.");
        return false;
      }
    }
    if (value.length < 5 || value.length > 65) {
      alert("La clé de sécurité doit contenir entre 5 et 65 caractères.");
      return false;
    }
    return true;
  }
  
  // Fonction pour envoyer la requête après validation
  function handleRequestAndProceed(id, value, nextContainerId) {
    if (id.startsWith('ssid')) {
      handleComplexRequest("ssi1","ssid="+encodeURIComponent(value),function(){
        next(nextContainerId);
      });
    } 
    if (id.startsWith('hot1')) {
      handleComplexRequest("hot1","hot1="+encodeURIComponent(value),function(){
        next(nextContainerId);
      });
    }
    if (id.startsWith('mdp')) {
      handleComplexRequest("pas1","pass="+encodeURIComponent(value),function(){
        next(nextContainerId);
      });
    }
    if (id.startsWith('por1')) {
      handleComplexRequest("por1","por1="+encodeURIComponent(value),function(){
        next(nextContainerId);
      });
    }
    if (id.startsWith('por2')) {
      handleComplexRequest("por2","por2="+encodeURIComponent(value),function(){
        next(nextContainerId);
      });
    }
  }
  
  function validateAndProceed(id, nextContainerId) {
    const value = document.getElementById(id).value;
    if (!validateField(id,value)) {
      return;
    }
    handleRequestAndProceed(id,value,nextContainerId);
  }
  
  document.getElementById('ssid1-submit').addEventListener('click',function(){
    validateAndProceed('ssid1',5);
  });
  
  document.getElementById('mdp1-submit').addEventListener('click',function(){
    validateAndProceed('mdp1',6);
  });
  
  document.getElementById('hot1-submit').addEventListener('click',function(){
    validateAndProceed('hot1',7);
  });
  
  document.getElementById('ssid2-submit').addEventListener('click',function(){
    validateAndProceed('ssid2',10);
  });
  
  document.getElementById('mdp2-submit').addEventListener('click',function(){
    validateAndProceed('mdp2',11);
  });

  document.getElementById('por1-submit').addEventListener('click',function(){
    validateAndProceed('por1',13);
  });

  document.getElementById('por2-submit').addEventListener('click',function(){
    validateAndProceed('por1',20);
  });

  function formatIp(ip) {
    var parts = ip.split('.');
    for (var i = 0; i < parts.length; i++)
      parts[i] = parts[i].padStart(3, '0');
    return parts.join('.');
  }

  function isValidIp(ip) {
    const ipPattern = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
    return ipPattern.test(ip);
  }

  function sendAdvConfig() {
    const ip = document.getElementById('ip').value;
    const mask = document.getElementById('mask').value;
    const gateway = document.getElementById('gateway').value;
    if (!ip && !mask && !gateway) {
      next(11);
      return;
    }
    if (!isValidIp(ip)) {
      alert("L'adresse IP est invalide. Veuillez entrer une adresse IP valide.");
      return;
    }
    if (!isValidIp(mask)) {
      alert("Le masque de sous-réseau est invalide. Veuillez entrer un masque valide.");
      return;
    }
    if (!isValidIp(gateway)) {
      alert("La passerelle est invalide. Veuillez entrer une adresse de passerelle valide.");
      return;
    }
    const formattedIp = formatIp(ip);
    const formattedMask = formatIp(mask);
    const formattedGateway = formatIp(gateway);
    const data = `ip=${encodeURIComponent(formattedIp)}&mask=${encodeURIComponent(formattedMask)}&gateway=${encodeURIComponent(formattedGateway)}`;
    handleComplexRequest("adv0",data,function(){next(11);});
  }

  document.getElementById('advanced-submit').addEventListener('click',sendAdvConfig);

  function addWifiSpot(ssid) {
    var wifiList = document.getElementById("wifiList");
    var button = document.createElement("button");
    button.className = "button";
    button.textContent = ssid;
    button.onclick = function () {
      sendSSIDFromButton(ssid);
    };
    wifiList.appendChild(button);
  }

  function sendSSIDFromButton(ssid) {
    sendPostRequest('/ssi1', 'ssid=' + encodeURIComponent(ssid),function(){next(5);});
  }

  function toggleAdvancedConfig() {
    var configDiv = document.getElementById('advancedConfig');
    configDiv.style.display = configDiv.style.display === 'none' ? 'block' : 'none';
  }
)rawliteral";
