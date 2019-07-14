import React from 'react';
import './App.css';
import {Switch, Route} from 'react-router-dom'
import Wifi from './pages/wifi'


/*
function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
      <div style={{width: "100%"}}>
        <WifiApList apList={[{name: "Fjell og sverd", signalStrength: 2}, {name: "Fjell og sverd 5G", signalStrength: 80}]}/>
      </div>
    </div>
  );
}
*/

function App() {
  return (
    <div className="App">
      <header className="App-header">
          <Switch>
            <Route exact path='/' component={Wifi} />
          </Switch>
      </header>
    </div>
  );
}

export default App;
