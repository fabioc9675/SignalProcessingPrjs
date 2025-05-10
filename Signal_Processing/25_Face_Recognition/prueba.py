import cv2
from deepface import DeepFace

cam = cv2.VideoCapture(0)

print("Presiona 'q' para salir...")

while True:
    ret, frame = cam.read()
    if not ret:
        break

    try:
        result = DeepFace.analyze(frame, actions=["emotion"], enforce_detection=False)[0]
        emotion = result["dominant_emotion"]
        cv2.putText(frame, f"{emotion}", (10, 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    except:
        pass

    cv2.imshow("Emociones", frame)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cam.release()
cv2.destroyAllWindows()
