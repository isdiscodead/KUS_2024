import logo from './logo.svg';
import './App.css';
import styled from 'styled-components';
import { useEffect, useState, useRef } from 'react';
import axios from 'axios';


function App() {
  const videoRef = useRef(null);
  const [time, setTime] = useState(0);
  const [isOn, setIsOn] = useState(false);
  const [bestTime, setBestTime] = useState(0);
  const [result, setResult] = useState(null); // 각 프레임 분석 결과
  const [xywh, setXywh] = useState([0, 0, 0, 0]); // 기준이 되는 xywh 
  const [isGood, setIsGood] = useState(true); // 자세 문제 여부 

  // 웹캠 권한 요청 및 스트림 설정
  const requestWebcamAccess = async () => {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({ video: true });
      console.log("Webcam access granted:", stream);

      // 비디오 태그에 스트림 연결
      if (videoRef.current) {
        videoRef.current.srcObject = stream;
      }
    } catch (err) {
      if (err.name === "NotAllowedError") {
        alert("웹캠 권한이 거부되었습니다. 브라우저 설정에서 권한을 허용하세요.");
      } else if (err.name === "NotFoundError") {
        alert("웹캠이 연결되어 있지 않습니다.");
      } else {
        console.error("Error accessing webcam:", err);
      }
    }
  };

  // 권한 상태 확인
  const checkPermissions = async () => {
    if (!navigator.permissions) {
      console.warn("Permissions API not supported in this browser.");
      return;
    }

    try {
      const cameraPermission = await navigator.permissions.query({ name: "camera" });
      console.log("Camera permission status:", cameraPermission.state);

      if (cameraPermission.state === "denied") {
        alert("웹캠 권한이 차단되었습니다. 브라우저 설정에서 권한을 허용하세요.");
      }
    } catch (err) {
      console.error("Error checking permissions:", err);
    }
  };

  // 프레임 캡처 및 전송
  const captureFrame = async () => {
    try {
      const canvas = document.createElement("canvas");
      const context = canvas.getContext("2d");

      // 비디오 해상도 설정
      canvas.width = videoRef.current.videoWidth;
      canvas.height = videoRef.current.videoHeight;

      // 캔버스에 비디오 프레임 그리기
      context.drawImage(videoRef.current, 0, 0, canvas.width, canvas.height);
      const frame = canvas.toDataURL("image/jpeg");

      // 디버깅: 캡처된 프레임 확인
      console.log("Captured frame:", frame.substring(0, 100)); // 너무 길면 앞부분만 출력

      // 서버로 프레임 전송
      const response = await axios.post(
        'http://localhost:8000/api/process_frame/',
        { frame },
        {
          headers: {
            'Content-Type': 'application/json',
          },
        }
      );
      console.log("Server response:", response.data);
      setResult(response.data.predictions); // 예측 결과를 상태로 저장
    } catch (err) {
      console.error("Error capturing or sending frame:", err);
    }
  };

  // 첫 실행 시 
  useEffect(() => {
    let storagedTime = localStorage.getItem('bestTime');
    storagedTime ? setBestTime(storagedTime) : setBestTime(0); // bestTime 초기화

    const initialize = async () => {
      await checkPermissions();
      await requestWebcamAccess();
    };

    initialize();

    const interval = setInterval(captureFrame, 1000); // 10FPS
    return () => clearInterval(interval);
  }, []);

  // 타이머 리셋 
  useEffect(() => {
    setTime(0);
  }, [isOn]);

  return (
    <main style={{ 'display' : 'flex', 'flexDirection' : 'column', 'alignItems': 'center' }}>
      <h2>바른 자세 공부 타이머 : 바공</h2>
      <div>가슴을 펴고 골반과 무릎은 90도 이상을 유지하세요!</div>
      <div>목을 바로 세우고 턱을 아래 안쪽으로 살짝 당겨요.</div><br/>
      <div>현재까지 최고 기록 : {bestTime}</div><br/>

      <div>
        <video ref={videoRef} autoPlay muted playsInline />
      </div>

      <TimerButton onClick={() => setIsOn(!isOn)}>{isOn ? '타이머 리셋하기' : '지금 자세로 시작하기'}</TimerButton>
      <TimePanel>{time}</TimePanel>

      <div>
        {result && result.map((item, index) => (
          <div key={index}>
            <p>Label: {item.label}</p>
            <p>Confidence: {item.confidence.toFixed(2)}</p>
            <p>BBox: {item.bbox.join(', ')}</p>
          </div>
        ))}
      </div>
    </main>
  );
}

const TimerButton = styled.button`
  padding: 0.5rem 2rem;
  border: none;
  background: pink;
  border-radius: 3rem;
  color: white;
  margin: 1rem;
`;

const TimePanel = styled.div``;

export default App;
