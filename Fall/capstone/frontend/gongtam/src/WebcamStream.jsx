import React, { useRef, useEffect, useState } from 'react';
import axios from 'axios';

const WebcamStream = () => {
  const videoRef = useRef(null);
  const [result, setResult] = useState(null);

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

  useEffect(() => {
    const initialize = async () => {
      await checkPermissions();
      await requestWebcamAccess();
    };

    initialize();

    const interval = setInterval(captureFrame, 100); // 10FPS
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <video ref={videoRef} autoPlay muted playsInline />
      <div>
        {result && result.map((item, index) => (
          <div key={index}>
            <p>Label: {item.label}</p>
            <p>Confidence: {item.confidence.toFixed(2)}</p>
            <p>BBox: {item.bbox.join(', ')}</p>
          </div>
        ))}
      </div>
    </div>
  );
};

export default WebcamStream;
