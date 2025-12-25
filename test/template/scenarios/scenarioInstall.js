var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@itspar/codepush-sdk";

module.exports = {
    startTest: function (testApp) {
        CodePushWrapper.checkAndInstall(testApp, undefined, undefined, CodePush.InstallMode.IMMEDIATE);
    },

    getScenarioName: function () {
        return "Install";
    }
};