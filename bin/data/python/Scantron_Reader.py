# from imutils.perspective import four_point_transform
# from imutils import contours
import numpy as np
import argparse
#import imutils
import cv2
import matplotlib as plt
from matplotlib import pyplot as plt
from pathlib import Path
import argparse
import time
from random import randint

import serial
from pythonosc import udp_client

ser =None
NoArduino =True
try:
    ser = serial.Serial('/dev/ttyUSB0')  # open serial port
    print(ser.name)
    NoArduino=False
except:
    print("Could not find Arduino for the serial button press. Just going to print out results.")
    ser=None

client = udp_client.SimpleUDPClient("127.0.01", 12345)



num_choices = 4
num_questions = 8
img_dir = "./test_Scantron/"
img_index = 0
webCam = False

sendBuffer=[]


def load_image():
    #read image on disk to buffer
    # Input Structure
    img_file_name = []

    entries = Path(img_dir)
    for entry in entries.iterdir():
        img_file_name.append(entry.name)
        #print(entry.name)

    image = cv2.imread(img_dir + img_file_name[img_index])
    image = cv2.rotate(image, cv2.ROTATE_90_COUNTERCLOCKWISE)
    print(img_file_name[img_index])

    # gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    # edged = cv2.Canny(blurred, 75, 200)
    # thresh = cv2.threshold(edged, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]

    print("Load in image at index: " + str(img_index))

    return image


def pre_processing(image):
    # grayscale, blur, edge detection
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    gray_img_eqhist=cv2.equalizeHist(gray)
    blurred = cv2.GaussianBlur(gray_img_eqhist, (5, 5), 0)
    edged = cv2.Canny(blurred, 5, 60)
    thresh = cv2.threshold(edged, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]

    return edged, thresh


def find_contours(edged):
    # find contours in the edge map, then initialize
    # the contour that corresponds to the document
    contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)
    #Changes based on https://stackoverflow.com/questions/53906178/how-opencv-4-x-api-is-different-from-previous-version
    #print(len(contours))

    cnt = contours
    #cv2.drawContours(image, cnt, 3, (0,255,0), 3)

    circle_x = []
    circle_y = []
    questionCnts = []

    for c in contours:

        #---- making sure to avoid small unwanted contours ---
        if cv2.contourArea(c) > 200:
            epsilon = cv2.arcLength(c, True)
            approximations = cv2.approxPolyDP(c, 0.04 * epsilon, True)
            area = cv2.contourArea(c)

            #--- selecting contours having more than 2 sides ---
            if  4 <= len(approximations) and area > 400 and area < 50000:
                (x, y, w, h) = cv2.boundingRect(c)
                ar = w / float(h)

                # in order to label the contour as a question, region
                # should be sufficiently wide, sufficiently tall, and
                # have an aspect ratio approximately equal to 1
                if w >= 20 and h >= 20 and ar >= 0.85 and ar <= 1.15:
                    questionCnts.append(c)

                    #(x,y),radius = cv2.minEnclosingCircle(c)
                    #x, y, w, h = cv2.boundingRect(c)
                    #print(x, y)

                circle_x.append(x)
                circle_y.append(y)

    return questionCnts



def sort_contours(cnts, method):
    # initialize the reverse flag and sort index
    reverse = False
    i = 0
    # handle if we need to sort in reverse
    if method == "right-to-left" or method == "bottom-to-top":
        reverse = True
    # handle if we are sorting against the y-coordinate rather than
    # the x-coordinate of the bounding box
    if method == "top-to-bottom" or method == "bottom-to-top":
        i = 1
    # construct the list of bounding boxes and sort them from top to
    # bottom
    boundingBoxes = [cv2.boundingRect(c) for c in cnts]
    (cnts, boundingBoxes) = zip(*sorted(zip(cnts, boundingBoxes),
        key=lambda b:b[1][i], reverse=reverse))
    # return the list of sorted contours and bounding boxes
    return (cnts, boundingBoxes)



def find_bubbled(questionCnts, thresh):
    # Output Structure
    circled_answer = []


    # Sort circles from top to down
    if len(questionCnts) > 0:
        questionCnts = sort_contours(questionCnts, "top-to-bottom")[0]

        # Go through each line of circles, sort from left to right, then find the shaded
        # one.
        for (q, i) in enumerate(np.arange(0, len(questionCnts), num_choices)):
            # sort the contours for the current question from
            # left to right, then initialize the index of the
            # bubbled answer
            cnts = sort_contours(questionCnts[i:i + num_choices], "left-to-right")[0]
            #print(cnts_tmp)
            bubbled = None
            #     cv2.drawContours(image3, cnts, -1, 255, -1)
            #     plt.imshow(image3)

            # loop over the sorted contours
            for (j, c) in enumerate(cnts):
                # construct a mask that reveals only the current
                # "bubble" for the question
                #print("j: " + str(j))
                mask = np.zeros(thresh.shape, dtype="uint8")
                cv2.drawContours(mask, [c], -1, 255, -1)

                # apply the mask to the thresholded image, then
                # count the number of non-zero pixels in the
                # bubble area
                mask = cv2.bitwise_and(thresh, thresh, mask=mask)
                total = cv2.countNonZero(mask)
                #print(total)

                # if the current total has a larger number of total
                # non-zero pixels, then we are examining the currently
                # bubbled-in answer
                if bubbled is None or total < bubbled[0]:
        #             print("updated bubbled")
        #             print(total)
        #             print(j)

                    bubbled = (total, j)
                    #print(len(bubbled))

            #print(bubbled)
            circled_answer.append(bubbled[1])
    #print("the lenth of circled answers before fill is: " + str(len(circled_answer)))
    client.send_message("/count", len(circled_answer))
    return circled_answer
    # cv2.drawContours(image3, [cnts[bubbled[1]]], -1, (255, 0, 0), 3)
    # plt.imshow(image3)

def process_frame(frame):
    global sendBuffer
    image_edged, image_thresh = pre_processing(frame)
    question_contours = find_contours(image_edged)
    response = find_bubbled(question_contours, image_thresh)
    if(len(response)==8):
        #print("UdatedGlobalBuffer")
        sendBuffer=response.copy()
    return response_safeguard(response)

# Make sure it shows proper number of responses
def response_safeguard(response):

    #check for too little recognized things
    while len(response) < num_questions:
        response.append(randint(0, num_choices-1))

    #check for too many recognized things
    if len(response) > num_questions:
        response = response[:num_questions]

    return response


# Keyboard Interrupt Handler
def _keyboardInterrupt_handler(signum, frame):
    print("  key board interrupt received...")
    cap.release()
    try:
        sys.exit(0)
    except SystemExit:
        os._exit(0)


if __name__ == "__main__":

    #------Load Image from disk or webcam-------#
    parser = argparse.ArgumentParser()
    parser.add_argument(
        'img_index', type=int, help='the image index', nargs="?", default = 0)
    parser.add_argument(
        "-t", "--test", action="store_true", help="use disk image")
    parser.add_argument(
    "-n", "--nodisplay", action="store_true", help="don't display an output window")

    args = parser.parse_args()

    #-----In Test mode. Use disk image based on image index. ------#
    if args.test:
        print("Test mode. Using image on disk.")
        img_index = args.img_index

        #------Process Image-------#
        image = load_image()
        image_edged, image_thresh = pre_processing(image)
        #image_edged, image_thresh = load_image()
        question_contours = find_contours(image_edged)
        response = find_bubbled(question_contours, image_thresh)

        print("Responses read in are: ")
        print(response)

    else:
        # Or use webcame
        try:
            print("Trying to open the Webcam.")
            cap = cv2.VideoCapture(0)
            if cap is None or not cap.isOpened():
               raise("No camera")
            webCam = True
        except:
            img_index = 0
            print("Using default image.")


        while(True):

            if webCam:
                #print("Starting loop")
                ret, img = cap.read()
                response = process_frame(img)

                if not args.nodisplay:
                    cv2.imshow("TESTING", img)

                if cv2.waitKey(1) & 0xFF == ord('q'):
                    cap.release()
                    break

                line=None
                if  (not ser==None )and( ser.is_open):
                    if(ser.in_waiting>0):
                        line = ser.readline()
                if line==None:
                    continue
                #print("Got a message not sure if its p");
                if  not 'p' in str(line):
                    print("... and it was not p :((((((" , line)
                    continue;
                if len(sendBuffer) == 8:
                    client.send_message("/pattern", sendBuffer)
                else:
                    print("Was trying to send data but didnt detect anything yet :( !")
            else:
                break


        if not args.nodisplay:
            cv2.destroyAllWindows()
