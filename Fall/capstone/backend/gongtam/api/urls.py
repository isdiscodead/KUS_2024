from django.urls import path
from .views import ProcessFrameView

urlpatterns = [
    path('api/process_frame/', ProcessFrameView.as_view(), name='process_frame'),
]