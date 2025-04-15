import logo from './logo.svg';
import Card from "./components/Card/card"
import './App.css';
import { useEffect, useState } from 'react';
import Axios from 'axios';

function App() {

  const [data, setData] = useState([]);
  useEffect(() => {
    Axios.get('/users').then(res => { setData(res.data) }).catch(e => { console.log(e); })
  })
  return (
    <div className="App">
      <header className="App-header">
        <Card name="Manish" email="manishverma@gmail.com"></Card>
        {data.map((e) => <Card name={e.name} email={e.email}></Card>)}

      </header>
    </div>
  );
}

export default App;
