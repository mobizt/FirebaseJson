#include <Arduino.h>
#include "FirebaseJson.h"

void setup()
{

    Serial.begin(115200);

    Serial.println();
    Serial.println();

    FirebaseJson json1;

    json1.setJsonData("{\"menu\":{\"header\":\"SVG Viewer\",\"items\":[{\"id\":\"Open\"},{\"id\":\"OpenNew\",\"label\":\"Open New\"},null,{\"id\":\"ZoomIn\",\"label\":\"Zoom In\"},{\"id\":\"ZoomOut\",\"label\":\"Zoom Out\"},{\"id\":\"OriginalView\",\"label\":\"Original View\"},null,{\"id\":\"Quality\"},{\"id\":\"Pause\"},{\"id\":\"Mute\"},null,{\"id\":\"Find\",\"label\":\"Find...\"},{\"id\":\"FindAgain\",\"label\":\"Find Again\"},{\"id\":\"Copy\"},{\"id\":\"CopyAgain\",\"label\":\"Copy Again\"},{\"id\":\"CopySVG\",\"label\":\"Copy SVG\"},{\"id\":\"ViewSVG\",\"label\":\"View SVG\"},{\"id\":\"ViewSource\",\"label\":\"View Source\"},{\"id\":\"SaveAs\",\"label\":\"Save As\"},null,{\"id\":\"Help\"},{\"id\":\"About\",\"label\":\"About Adobe CVG Viewer...\"}]}}");

    FirebaseJsonArray arr1;
    arr1.set("[0]/int", 123);
    arr1.set("[0]/double", 456.789);
    arr1.set("[0]/string", "Hello World!");
    arr1.set("[0]/bool", true);
    arr1.set("[1]/int", 876);
    arr1.set("[2]/[1]/string", "done");
    arr1.set(4, "What's up");

    FirebaseJson json2;
    json2.set("Hi/myInt", 200);
    json2.set("Hi/myDouble", 0.0023);
    json2.set("This/is/[2]/[3]", "Me");
    json2.set("That/is", false);

    arr1.set(6, json2);

    FirebaseJsonData jsonObj;

    json1.set("anotherNode/data/[0]", "How are you?");
    json1.set("anotherNode/data/[2]", arr1);

    json1.get(jsonObj, "menu/items/[5]/label");

    Serial.println("-------------------------");

    Serial.println(json1.toString(true));

    Serial.println("-------------------------");

    Serial.println(jsonObj.stringValue);

    Serial.println("-------------------------");

    json1.remove("menu/items");

    Serial.println(json1.toString(true));

    Serial.println("-------------------------");

    json1.set("anotherNode/data/[6]/That/is", "my house");
    json1.get(jsonObj, "anotherNode/data/[6]/That/is");

    Serial.println(jsonObj.stringValue);

    Serial.println("-------------------------");

    Serial.println(json1.toString(true));
}

void loop()
{
}