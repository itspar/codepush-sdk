var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@itspar/codepush-sdk";

module.exports = {
    startTest: function (testApp) {
        CodePush.disallowRestart();
        CodePushWrapper.checkAndInstall(testApp,
            undefined,
            undefined,
            CodePush.InstallMode.ON_NEXT_SUSPEND,
            undefined,
            true
        );
    },

    getScenarioName: function () {
        return "disallowRestart";
    }
};
