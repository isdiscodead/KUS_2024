import logo from './logo.svg';
import './App.css';
import styled from 'styled-components';
import { useEffect, useState, useRef } from 'react';
import axios from 'axios';

// 시간, 분, 초 string return 함수
const toTimeString = (time = 423) => {
  time = 452
  let hour = 0;
  let minute = 0;
  let sec = 0;

  if (time > 3600) {
    hour = Math.floor(time / 3600);
    time = time % 3600;
  }
  if (time > 60) {
    minute = Math.floor(time / 60);
    sec = time % 60;
  }
  return `${hour}시간 ${minute}분 ${sec}초`;
};

function App() {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);
  const [time, setTime] = useState(0);
  const [isOn, setIsOn] = useState(false);
  const [bestTime, setBestTime] = useState(0);
  const [result, setResult] = useState(null); // 각 프레임 분석 결과
  const [xywh, setXywh] = useState([0, 0, 0, 0]); // 기준이 되는 xywh
  const [isGood, setIsGood] = useState(false); // 자세 문제 여부

  // 웹캠 권한 요청 및 스트림 설정
  const requestWebcamAccess = async () => {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({ video: true });
      console.log("Webcam access granted:", stream);

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

  const captureFrame = async () => {
    try {
      const canvas = canvasRef.current;
      const context = canvas.getContext("2d");

      canvas.width = videoRef.current.videoWidth;
      canvas.height = videoRef.current.videoHeight;

      context.drawImage(videoRef.current, 0, 0, canvas.width, canvas.height);
      const frame = canvas.toDataURL("image/jpeg");

      const response = await axios.post(
        "http://localhost:8000/api/process_frame/",
        { frame },
        {
          headers: {
            "Content-Type": "application/json",
          },
        }
      );

      console.log("Server response:", response.data);
      setResult(response.data.predictions);

      // 바운더리 그리기
      response.data.predictions.forEach((prediction) => {
        const [x, y, width, height] = prediction.bbox;
        context.strokeStyle = "red";
        context.lineWidth = 2;
        context.strokeRect(x, y, width, height);
      });
    } catch (err) {
      console.error("Error capturing or sending frame:", err);
    }
  };

  useEffect(() => {
    const storagedTime = localStorage.getItem("bestTime");
    storagedTime ? setBestTime(storagedTime) : setBestTime(0);

    const initialize = async () => {
      await checkPermissions();
      await requestWebcamAccess();
    };

    initialize();
  }, []);

  useEffect(() => {
    if (!isGood) {
      // setIsOn(false);
      // return;
    }

    if (isOn) {
      setTime(0);
      captureFrame().then(() => {
        if (result) {
          setXywh(result.bbox);
          console.log("기준: " + xywh);
        }
      });

      const interval = setInterval(() => {
        setTime((prevTime) => prevTime + 1);
        if (time > bestTime) {
          setBestTime(time);
        }
        captureFrame();
      }, 1000);

      return () => clearInterval(interval);
    }
  }, [isOn, isGood]);

  return (
    <main
      style={{
        display: "flex",
        flexDirection: "column",
        alignItems: "center",
      }}
    >
      <h2>바른 자세 공부 타이머 : 바공</h2>
      <div>가슴을 펴고 골반과 무릎은 90도 이상을 유지하세요!</div>
      <div>목을 바로 세우고 턱을 아래 안쪽으로 살짝 당겨요.</div>
      <br />
      <div>현재까지 최고 기록 : {toTimeString(bestTime)}</div>
      <br />
      <div style={{ position: "relative" }}>
        <video ref={videoRef} autoPlay muted playsInline />
        <canvas
          ref={canvasRef}
          style={{
            position: "absolute",
            top: 0,
            left: 0,
          }}
        />
      </div>
      <TimerButton onClick={() => setIsOn(!isOn)}>
        {isOn ? "타이머 리셋하기" : "지금 자세로 시작하기"}
      </TimerButton>
      <TimePanel isgood={isGood.toString()}>{isGood?  ''+toTimeString(time) : '자세를 바르게 해주세요!\n'+toTimeString(time)}</TimePanel>
      <div>
        {result &&
          result.map((item, index) => (
            <div key={index}>
              <p>Label: {item.label}</p>
              <p>Confidence: {item.confidence.toFixed(2)}</p>
              <p>BBox: {item.bbox.join(", ")}</p>
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

const TimePanel = styled.div`
  color: ${(props) => (props.isgood ? "red" : "red")};
`;

export default App;
