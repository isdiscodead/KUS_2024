from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework.parsers import JSONParser
from ultralytics import YOLO
import base64
from PIL import Image
from io import BytesIO

class ProcessFrameView(APIView):
    parser_classes = [JSONParser]

    def post(self, request, *args, **kwargs):
        frame_data = request.data.get('frame')
        if not frame_data:
            return Response({'error': 'No frame data provided'}, status=400)

        # 디버깅: 받은 데이터 출력
        print("Received frame_data:", frame_data[:100])  # 너무 길면 앞부분만 출력
        
        # Base64 프리픽스 제거
        image_data = frame_data.split(",")[1]
        decoded_image = base64.b64decode(image_data)

        # PIL 이미지 로드 및 검증
        image = Image.open(BytesIO(decoded_image))
        image.verify()  # 이미지 검증
        image = Image.open(BytesIO(decoded_image))  # 재로드

        # YOLOv10 모델 추론
        model = YOLO("yolov10n.pt")  # 모델 로드
        results = model.predict(image)  # 추론 수행

        # 결과 변환 및 반환
        response_data = {"detections": results}
        return Response(response_data)
    
    def get(self, request, *args, **kwargs):
        return Response('hi')