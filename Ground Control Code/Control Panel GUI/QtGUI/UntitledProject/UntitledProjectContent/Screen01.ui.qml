

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import UntitledProject
import QtQuick.Studio.DesignEffects
import QtQuick.Studio.Components 1.0
import QtQuick.Timeline 1.0

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height

    color: Constants.backgroundColor

    Image {
        id: bG
        x: 0
        y: 0
        source: "images/BGNew.png"
        fillMode: Image.PreserveAspectFit

        Image {
            id: signboard
            x: -36
            y: 23
            visible: true
            source: "images/Signboard.png"
            fillMode: Image.PreserveAspectFit
        }

        GroupItem {
            id: rpmreadouts
            x: 1561
            y: 871
            width: 315
            height: 235

            Text {
                id: rpmcount
                x: 36
                y: -10
                width: 143
                height: 138
                color: "#fdb400"
                text: qsTr("95k")
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Black
                font.family: "Infynyte Body"
                Connections {
                    target: backend
                    function onRpm_data_received(data) {
                        if (data > 1000) {
                            rpmcount.text = (data / 1000).toFixed(1) + "k"
                        } else {
                            rpmcount.text = data
                        }
                    }
                }

                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#da9a00"
                            blur: 28
                        }
                    ]
                }
            }

            Image {
                id: fanBlades
                x: 55
                y: 12
                width: 103
                height: 103
                opacity: 0.1
                source: "images/Fan Blades.png"
                rotation: 360
                fillMode: Image.PreserveAspectFit

                Image {
                    id: fanCore
                    x: 38
                    y: 41
                    width: 28
                    height: 22
                    source: "images/Fan Core.png"
                    fillMode: Image.PreserveAspectFit
                }

                NumberAnimation {
                    id: number
                    target: fanBlades
                    property: "rotation"
                    alwaysRunToEnd: true
                    running: true
                    duration: 2500
                    loops: -1
                    to: 360
                    from: 0
                }
            }
            Text {
                id: text8
                x: 50
                y: 48
                width: 114
                height: 113
                color: "#fdb400"
                text: qsTr("RPM")
                font.pixelSize: 23
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.ExtraBold
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#da9a00"
                            blur: 28
                        }
                    ]
                }
            }

            GroupItem {
                id: flowrate
                x: 131
                y: -229

                Text {
                    id: text9
                    x: 0
                    y: 0
                    width: 114
                    height: 113
                    color: "#fdb400"
                    text: qsTr("3.9")
                    font.pixelSize: 62
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.weight: Font.ExtraBold
                    font.family: "Infynyte Body"
                    DesignEffect {
                        effects: [
                            DesignDropShadow {
                                color: "#da9a00"
                                blur: 28
                            }
                        ]
                    }
                }

                Text {
                    id: text10
                    x: 0
                    y: 58
                    width: 114
                    height: 113
                    color: "#adff00"
                    text: qsTr("3.7")
                    font.pixelSize: 37
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.weight: Font.ExtraBold
                    font.family: "Infynyte Body"
                    DesignEffect {
                        effects: [
                            DesignDropShadow {
                                color: "#adff00"
                                blur: 28
                            }
                        ]
                    }
                }

                Text {
                    id: text11
                    x: 55
                    y: 64
                    width: 114
                    height: 113
                    color: "#adff00"
                    text: qsTr("L/min")
                    font.pixelSize: 25
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.weight: Font.ExtraBold
                    font.family: "Infynyte Body"
                    DesignEffect {
                        effects: [
                            DesignDropShadow {
                                color: "#adff00"
                                blur: 28
                            }
                        ]
                    }
                }

                Text {
                    id: text12
                    x: 75
                    y: 12
                    width: 114
                    height: 113
                    color: "#ffb400"
                    text: qsTr("L/min")
                    font.pixelSize: 25
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.weight: Font.ExtraBold
                    font.family: "Infynyte Body"
                    DesignEffect {
                        effects: [
                            DesignDropShadow {
                                color: "#ffb400"
                                blur: 28
                            }
                        ]
                    }
                }

                ArcItem {
                    id: current
                    x: -56
                    y: -34
                    width: 268
                    height: 268
                    antialiasing: true
                    strokeColor: "#ffb400"
                    end: 36
                    begin: -90
                    strokeWidth: 31
                    fillColor: "#00000000"

                    DesignEffect {}
                }

                ArcItem {
                    id: target
                    x: -25
                    y: -3
                    width: 206
                    height: 206
                    strokeWidth: 8
                    strokeColor: "#adff00"
                    fillColor: "#00000000"
                    end: 36
                    DesignEffect {}
                    begin: -90
                    antialiasing: true
                }
            }

            Text {
                id: newtons
                x: 195
                y: -10
                width: 143
                height: 138
                color: "#fdb400"
                text: qsTr("25")
                font.pixelSize: 50
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Black
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#da9a00"
                            blur: 28
                        }
                    ]
                }
            }

            Text {
                id: text13
                x: 209
                y: 48
                width: 114
                height: 113
                color: "#fdb400"
                text: qsTr("Newtons")
                font.pixelSize: 23
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.ExtraBold
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#da9a00"
                            blur: 28
                        }
                    ]
                }
            }
        }

        GroupItem {
            id: temp
            x: 1067
            y: 545

            Image {
                id: s
                x: 63
                y: 63
                source: "images/S.png"
                fillMode: Image.PreserveAspectFit
                visible: false

                NumberAnimation {
                    id: number3
                    target: s
                    property: "rotation"
                    running: true
                    duration: 10000
                    loops: -1
                    to: 360
                    from: 0
                }
            }

            Image {
                id: m
                x: 33
                y: 33
                source: "images/M.png"
                fillMode: Image.PreserveAspectFit
                visible: false

                NumberAnimation {
                    id: number2
                    target: m
                    property: "rotation"
                    loops: -1
                    duration: 10000
                    running: true
                    to: 360
                    from: 0
                }
            }

            Image {
                id: l
                x: 0
                y: 0
                visible: false
                source: "images/L.png"
                state: ""
                fillMode: Image.PreserveAspectFit

                NumberAnimation {
                    id: number1
                    target: l
                    property: "rotation"
                    running: true
                    duration: 10000
                    loops: -1
                    to: 360
                    from: 0
                }
            }

            Text {
                id: topnum
                x: 152
                y: 63
                width: 114
                height: 113
                color: "#e45e06"
                text: qsTr("1920")
                font.pixelSize: 31
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.ExtraBold
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#e45e06"
                            blur: 28
                        }
                    ]
                }
            }

            Text {
                id: topnum1
                x: 100
                y: 182
                width: 114
                height: 113
                color: "#e45e06"
                text: qsTr("1080")
                font.pixelSize: 31
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.ExtraBold
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#e45e06"
                            blur: 28
                        }
                    ]
                }
            }

            Text {
                id: topnum2
                x: 198
                y: 182
                width: 114
                height: 113
                color: "#e45e06"
                text: qsTr("1070")
                font.pixelSize: 31
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.ExtraBold
                font.family: "Infynyte Body"
                DesignEffect {
                    effects: [
                        DesignDropShadow {
                            color: "#e45e06"
                            blur: 28
                        }
                    ]
                }
            }

            GroupItem {
                id: sbutt
                x: 52
                y: 47

                RadioButton {
                    id: sl
                    x: 0
                    y: 186
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.small_radio_buttons_clicked(sl.id)
                        sl.checked = true
                        su.checked = true
                        sr.checked = true

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = true
                        m.visible = false
                        l.visible = false
                    }
                }

                RadioButton {
                    id: sr
                    x: 242
                    y: 232
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.small_radio_buttons_clicked(sr.id)
                        sl.checked = true
                        su.checked = true
                        sr.checked = true

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = true
                        m.visible = false
                        l.visible = false
                    }
                }

                RadioButton {
                    id: su
                    x: 161
                    y: 0
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.small_radio_buttons_clicked(su.id)
                        sl.checked = true
                        su.checked = true
                        sr.checked = true

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = true
                        m.visible = false
                        l.visible = false
                    }
                }

                RadioButton {
                    id: ml
                    x: -23
                    y: 210
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.medium_radio_buttons_clicked(ml.id)
                        ml.checked = true
                        mu.checked = true
                        mr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = false
                        m.visible = true
                        l.visible = false
                    }
                }

                RadioButton {
                    id: ll
                    x: -47
                    y: 234
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.large_radio_buttons_clicked(ll.id)
                        ll.checked = true
                        lu.checked = true
                        lr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        s.visible = false
                        m.visible = false
                        l.visible = true
                    }
                }

                RadioButton {
                    id: mu
                    x: 152
                    y: -33
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.medium_radio_buttons_clicked(mu.id)
                        ml.checked = true
                        mu.checked = true
                        mr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = false
                        m.visible = true
                        l.visible = false
                    }
                }

                RadioButton {
                    id: lu
                    x: 143
                    y: -66
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.large_radio_buttons_clicked(lu.id)
                        ll.checked = true
                        lu.checked = true
                        lr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        s.visible = false
                        m.visible = false
                        l.visible = true
                    }
                }

                RadioButton {
                    id: mr
                    x: 275
                    y: 240
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.medium_radio_buttons_clicked(mr.id)
                        ml.checked = true
                        mu.checked = true
                        mr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ll.checked = false
                        lu.checked = false
                        lr.checked = false

                        s.visible = false
                        m.visible = true
                        l.visible = false
                    }
                }

                RadioButton {
                    id: lr
                    x: 307
                    y: 249
                    text: qsTr("")
                    checked: false
                    autoExclusive: false
                    onClicked: {
                        backend.large_radio_buttons_clicked(lr.id)
                        ll.checked = true
                        lu.checked = true
                        lr.checked = true

                        sl.checked = false
                        su.checked = false
                        sr.checked = false

                        ml.checked = false
                        mu.checked = false
                        mr.checked = false

                        s.visible = false
                        m.visible = false
                        l.visible = true
                    }
                }
            }
        }

        GroupItem {
            x: 1415
            y: 120

            Text {
                id: portlabel
                x: 0
                y: 0
                width: 138
                color: "#adff00"
                text: qsTr("Port:")
                font.pixelSize: 37
                horizontalAlignment: Text.AlignRight
                font.family: "Noto Serif Display"
            }

            TextInput {
                id: textInput
                x: 144
                y: 8
                width: 178
                height: 35
                color: "#aeff00"
                text: qsTr("COM3")
                font.pixelSize: 28
                selectionColor: "#6464ff"
                font.bold: true
                font.family: "Infynyte Body"

                Rectangle {
                    id: rectangle1
                    x: -5
                    y: -8
                    width: 112
                    height: 51
                    color: "#00ffffff"
                    radius: 11
                    border.color: "#aeff00"
                }
            }

            Text {
                id: portlabel1
                x: 0
                y: 57
                width: 138
                color: "#adff00"
                text: qsTr("Baud:")
                font.pixelSize: 37
                horizontalAlignment: Text.AlignRight
                font.family: "Noto Serif Display"
            }

            TextInput {
                id: textInput1
                x: 144
                y: 65
                width: 178
                height: 35
                color: "#aeff00"
                text: qsTr("115200")
                font.pixelSize: 28
                selectionColor: "#6464ff"
                font.family: "Infynyte Body"
                font.bold: true
                Rectangle {
                    id: rectangle2
                    x: -5
                    y: -8
                    width: 161
                    height: 51
                    color: "#00ffffff"
                    radius: 11
                    border.color: "#aeff00"
                }
            }
        }

        Button {
            id: button
            x: 1458
            y: 234
            width: 237
            height: 67
            text: qsTr("Connect")
            icon.color: "#ddff0000"
            font.pointSize: 30
            font.family: "Noto Serif Display"
            highlighted: false
            flat: false
            background: Rectangle {
                color: "#FFFF00"
                radius: 5
            }
            contentItem: Text {
                text: button.text
                font: button.font
                color: "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                backend.connect_button_clicked(textInput.text, textInput1.text)
            }

            states: [
                State {
                    name: "connected"
                    PropertyChanges {
                        target: button.background
                        color: "green"
                    }
                },
                State {
                    name: "error"
                    PropertyChanges {
                        target: button.background
                        color: "red"
                    }
                }
            ]

            Connections {
                target: backend
                function onConnection_successful() {
                    button.state = "connected"
                }
                function onConnection_error() {
                    button.state = "error"
                }
            }
        }
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: fanspin
                duration: 2500
                running: true
                loops: -1
                to: 150
                from: 0
            }
        ]
        startFrame: 0
        endFrame: 600
        enabled: true
    }
    states: [
        State {
            name: "clicked"
        }
    ]
}
