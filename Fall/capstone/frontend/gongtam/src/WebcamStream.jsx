import React, { useRef, useEffect, useState } from 'react';

const WebcamStream = () => {
  const videoRef = useRef(null);
  const [result, setResult] = useState(null);

  const captureFrame = async () => {
    const canvas = document.createElement("canvas");
    const context = canvas.getContext("2d");
    canvas.width = videoRef.current.videoWidth;
    canvas.height = videoRef.current.videoHeight;
    context.drawImage(videoRef.current, 0, 0, canvas.width, canvas.height);
    const frame = canvas.toDataURL("image/jpeg");

    // 서버로 프레임 전송
    const response = await fetch('http://localhost:8000/api/process_frame/', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ frame }),
    });
    const data = await response.json();
    setResult(data);
  };

  useEffect(() => {
    const interval = setInterval(captureFrame, 100); // 10FPS
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <video ref={videoRef} autoPlay muted playsInline />
      <div>{result && JSON.stringify(result)}</div>
    </div>
  );
};

export default WebcamStream;
