import React, { useState, useEffect } from 'react';
import './App.css';

const api = 'http://localhost:3001/'

const bal = <img src="data:image/png;base64,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"/>
const rel = <img src="https://img.icons8.com/glyph-neue/64/undefined/restart.png"/>
function App() {
  
  const [meassurement, setMeasurement] = useState(0);

  useEffect(() => {
    fetch(api, {
      mode: 'cors'
    })
    .then((response) => {
      if (!response.ok) {
        throw new Error(
          `This is an HTTP error: The status is ${response.status} `
        );
      }
      return response.json();
    })
    .then((actualData) => {
      console.log(actualData)
      setMeasurement(actualData)
    })
    .catch((err) => {
      console.log(err.message);
    });
  }, []);
  
  const reload = () => {
    fetch(api, {
      mode: 'cors'
    })
    .then((response) => {
      if (!response.ok) {
        throw new Error(
          `This is an HTTP error: The status is ${response.status}`
        );
      }
      return response.json();
    })
    .then((actualData) => {
      console.log(actualData)
      setMeasurement(actualData)
    })
    .catch((err) => {
      console.log(err.message);
    });
  }

  return (
    <>
      <div className='logoo'>
        {bal}
      </div>
      <div className='headr'>
        REACT - MQTT
      </div>
      <div className='hub'>
        Peso: 
        <div className='area'>
          <p>{meassurement}</p>
          <i className='rel' onClick={reload}>{rel}</i>
        </div>
      </div>
    </>
  );
}

export default App;
