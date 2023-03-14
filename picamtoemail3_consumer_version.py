import os
import smtplib
import datetime
import picamera
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.utils import COMMASPACE
from email import encoders
import time

# Email configuration
to = "email_goes_here" # Replace with your email address
subject = "Raspberry Pi photo taken on " + str(datetime.datetime.now()) #replace with your own subject title if desired
smtp_server = "smtp.gmail.com" # Replace with your SMTP server address
smtp_port = 587
smtp_username = "username_goes_here" # Replace with your SMTP username
smtp_password = "password_goes_here" # Replace with your SMTP password

# Camera configuration
image_file = "photo.jpg" #you can change the photo file name if needed
camera = picamera.PiCamera()
camera.resolution = (1024, 768)

while True:
    # Take photo
    camera.start_preview()
    # Add a delay to allow camera to adjust to lighting conditions
    camera.exposure_mode = 'auto'
    camera.awb_mode = 'auto'
    camera.meter_mode = 'matrix'
    camera.image_effect = 'none'
    camera.iso = 100
    camera.annotate_text = "Plant Status Update" # Text that goes on image, optional
    camera.annotate_text_size = 50
    camera.capture(image_file)
    camera.stop_preview()

    # Rotate photo
    os.system("convert {} -rotate 270 {}".format(image_file, image_file)) #rotates photo before sending, change degree of rotation to whatever is needed in your usecase 

    # Send email with attachment
    msg = MIMEMultipart()
    msg['From'] = smtp_username
    msg['To'] = to
    msg['Subject'] = subject

    with open(image_file, 'rb') as f:
        attachment = MIMEBase('application', 'octet-stream')
        attachment.set_payload(f.read())
        encoders.encode_base64(attachment)
        attachment.add_header('Content-Disposition', 'attachment', filename=os.path.basename(image_file))
        msg.attach(attachment)
    #Text that does in contents opf email, you can chage it to anything, but I find these useful
    text = "A photo taken by Raspberry Pi on " + str(datetime.datetime.now()) + "\n\n"
    text += "Look out for these things when viewing your plant:\n"
    text += "- Brown tips on leaves\n"
    text += "- Entirely brown leaves\n"
    text += "- Dry edges on leaves\n"
    text += "- Curling leaves\n"
    text += "- Stunted growth\n\n"
    text += "If any of these occur, human intervention or program modification may be required."
    msg.attach(MIMEText(text))

    smtp = smtplib.SMTP(smtp_server, smtp_port)
    smtp.starttls()
    smtp.login(smtp_username, smtp_password)
    smtp.sendmail(smtp_username, COMMASPACE.join([to]), msg.as_string())
    smtp.quit()

    # Delete photo file
    os.remove(image_file)

    # Wait for 30 minutes before taking the next photo
    time.sleep(1800)