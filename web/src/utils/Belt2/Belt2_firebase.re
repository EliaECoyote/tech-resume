[%bs.raw {| require("firebase/app") |}];
[%bs.raw {| require("firebase/auth") |}];
[%bs.raw {| require("firebase/firestore") |}];

let firebaseOptions: Firebase.firebaseOptions = {
  apiKey: Some("AIzaSyBfPVUI614vItgOep-x9ZGCgJZSV4HRcoI"),
  authDomain: Some("tech-resume.firebaseapp.com"),
  databaseURL: Some("https://tech-resume.firebaseio.com"),
  projectId: Some("tech-resume"),
  storageBucket: Some("tech-resume.appspot.com"),
  messagingSenderId: Some("588623468101"),
  appId: Some("1:588623468101:web:389a78e5de59b71eea752c"),
  measurementId: Some("G-NX8T2211M9"),
};

let init = () => Firebase.initializeApp(~firebaseOptions);