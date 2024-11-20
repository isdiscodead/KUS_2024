import logo from './logo.svg';
import './App.css';
import WebcamStream from './WebcamStream';

function App() {
  return (
    <main style={{ 'display' : 'flex', 'flexDirection' : 'column', 'alignItems': 'center' }}>
      <h2>어쩌고저쩌고</h2>
      <WebcamStream />
    </main>
  );
}

export default App;
