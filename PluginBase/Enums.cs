using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DotSpeak
{
    public enum PluginTargetMode : int
    {
        /// <summary>
        /// send plugincmd to all clients in current channel
        /// </summary>
        CurrentChannel = 0,
        /// <summary>
        /// send plugincmd to all clients on server
        /// </summary>
        Server,
        /// <summary>
        /// send plugincmd to all given client ids
        /// </summary>
        Client,
        /// <summary>
        /// send plugincmd to all subscribed clients in current channel
        /// </summary>
        CurrentChannelSubscribedClients,
        Max
    };

    public enum TS3Error : uint
    {
        //general
        Ok = 0x0000,
        Undefined = 0x0001,
        NotImplemented = 0x0002,
        OkNoUpdate = 0x0003,
        DontNotify = 0x0004,
        LibTimeLimitReached = 0x0005,

        //dunno
        CommandNotFound = 0x0100,
        UnableToBindNetworkPort = 0x0101,
        NoNetworkPortAvailable = 0x0102,
        PortAlreadyInUse = 0x0103,

        //client
        ClientInvalidID = 0x0200,
        ClientNicknameInuse = 0x0201,
        ClientProtocolLimitReached = 0x0203,
        ClientInvalidType = 0x0204,
        ClientAlreadySubscribed = 0x0205,
        ClientNotLoggedIn = 0x0206,
        ClientCouldNotValidateIdentity = 0x0207,
        ClientVersionOutdated = 0x020a,
        ClientIsFlooding = 0x020c,
        ClientHacked = 0x020d,
        ClientCannotVerifyNow = 0x020e,
        ClientLoginNotPermitted = 0x020f,
        ClientNotSubscribed = 0x0210,

        //channel
        ChannelInvalidID = 0x0300,
        ChannelProtocolLimitReached = 0x0301,
        ChannelAlreadyIn = 0x0302,
        ChannelNameInuse = 0x0303,
        ChannelNotEmpty = 0x0304,
        ChannelCanNotDeleteDefault = 0x0305,
        ChannelDefaultRequirePermanent = 0x0306,
        ChannelInvalidFlags = 0x0307,
        ChannelParentNotPermanent = 0x0308,
        ChannelMaxclientsReached = 0x0309,
        ChannelMaxfamilyReached = 0x030a,
        ChannelInvalidOrder = 0x030b,
        ChannelNoFiletransferSupported = 0x030c,
        ChannelInvalidPassword = 0x030d,
        //note 0x030e is defined in rare section below
        ChannelInvalidSecurityHash = 0x030f, 

        //server
        ServerInvalidID = 0x0400,
        ServerRunning = 0x0401,
        ServerIsShuttingDown = 0x0402,
        ServerMaxclientsReached = 0x0403,
        ServerInvalidPassword = 0x0404,
        ServerIsVirtual = 0x0407,
        ServerIsNotRunning = 0x0409,
        ServerIsBooting = 0x040a,
        ServerStatusInvalid = 0x040b,
        ServerVersionOutdated = 0x040d,
        ServerDuplicateRunning = 0x040e,

        //parameter
        ParameterQuote = 0x0600,
        ParameterInvalidCount = 0x0601,
        ParameterInvalid = 0x0602,
        ParameterNotFound = 0x0603,
        ParameterConvert = 0x0604,
        ParameterInvalidSize = 0x0605,
        ParameterMissing = 0x0606,
        ParameterChecksum = 0x0607,

        //unsorted, need further investigation
        VsCritical = 0x0700,
        ConnectionLost = 0x0701,
        NotConnected = 0x0702,
        NoCachedConnectionInfo = 0x0703,
        CurrentlyNotPossible = 0x0704,
        FailedConnectionInitialisation = 0x0705,
        CouldNotResolveHostname = 0x0706,
        InvalidServerConnectionHandlerID = 0x0707,
        CouldNotInitialiseInputManager = 0x0708,
        ClientlibraryNotInitialised = 0x0709,
        ServerlibraryNotInitialised = 0x070a,
        WhisperTooManyTargets = 0x070b,
        WhisperNoTargets = 0x070c,
        ConnectionIpProtocolMissing = 0x070d,

        //file transfer
        FileInvalidName = 0x0800,
        FileInvalidPermissions = 0x0801,
        FileAlreadyExists = 0x0802,
        FileNotFound = 0x0803,
        FileIoError = 0x0804,
        FileInvalidTransferID = 0x0805,
        FileInvalidPath = 0x0806,
        FileNoFilesAvailable = 0x0807,
        FileOverwriteExcludesResume = 0x0808,
        FileInvalidSize = 0x0809,
        FileAlreadyInUse = 0x080a,
        FileCouldNotOpenConnection = 0x080b,
        FileNoSpaceLeftOnDevice = 0x080c,
        FileExceedsFileSystemMaximumSize = 0x080d,
        FileTransferConnectionTimeout = 0x080e,
        FileConnectionLost = 0x080f,
        FileExceedsSuppliedSize = 0x0810,
        FileTransferComplete = 0x0811,
        FileTransferCanceled = 0x0812,
        FileTransferInterrupted = 0x0813,
        FileTransferServerQuotaExceeded = 0x0814,
        FileTransferClientQuotaExceeded = 0x0815,
        FileTransferReset = 0x0816,
        FileTransferLimitReached = 0x0817,

        //sound
        SoundPreprocessorDisabled = 0x0900,
        SoundInternalPreprocessor = 0x0901,
        SoundInternalEncoder = 0x0902,
        SoundInternalPlayback = 0x0903,
        SoundNoCaptureDeviceAvailable = 0x0904,
        SoundNoPlaybackDeviceAvailable = 0x0905,
        SoundCouldNotOpenCaptureDevice = 0x0906,
        SoundCouldNotOpenPlaybackDevice = 0x0907,
        SoundHandlerHasDevice = 0x0908,
        SoundInvalidCaptureDevice = 0x0909,
        SoundInvalidPlaybackDevice = 0x090a,
        SoundInvalidWave = 0x090b,
        SoundUnsupportedWave = 0x090c,
        SoundOpenWave = 0x090d,
        SoundInternalCapture = 0x090e,
        SoundDeviceInUse = 0x090f,
        SoundDeviceAlreadyRegisterred = 0x0910,
        SoundUnknownDevice = 0x0911,
        SoundUnsupportedFrequency = 0x0912,
        SoundInvalidChannelCount = 0x0913,
        SoundReadWave = 0x0914,
        SoundNeedMoreData = 0x0915, //for internal purposes only
        SoundDeviceBusy = 0x0916, //for internal purposes only
        SoundNoData = 0x0917,
        SoundChannelMaskMismatch = 0x0918,


        //permissions
        PermissionsClientInsufficient = 0x0a08,
        Permissions = 0x0a0c,

        //accounting
        AccountingVirtualserverLimitReached = 0x0b00,
        AccountingSlotLimitReached = 0x0b01,
        AccountingLicenseFileNotFound = 0x0b02,
        AccountingLicenseDateNotOk = 0x0b03,
        AccountingUnableToConnectToServer = 0x0b04,
        AccountingUnknownError = 0x0b05,
        AccountingServerError = 0x0b06,
        AccountingInstanceLimitReached = 0x0b07,
        AccountingInstanceCheckError = 0x0b08,
        AccountingLicenseFileInvalid = 0x0b09,
        AccountingRunningElsewhere = 0x0b0a,
        AccountingInstanceDuplicated = 0x0b0b,
        AccountingAlreadyStarted = 0x0b0c,
        AccountingNotStarted = 0x0b0d,
        AccountingToManyStarts = 0x0b0e,

        //provisioning server
        ProvisioningInvalidPassword = 0x1100,
        ProvisioningInvalidRequest = 0x1101,
        ProvisioningNoSlotsAvailable = 0x1102,
        ProvisioningPoolMissing = 0x1103,
        ProvisioningPoolUnknown = 0x1104,
        ProvisioningUnknownIpLocation = 0x1105,
        ProvisioningInternalTriesExceeded = 0x1106,
        ProvisioningTooManySlotsRequested = 0x1107,
        ProvisioningTooManyReserved = 0x1108,
        ProvisioningCouldNotConnect = 0x1109,
        ProvisioningAuthServerNotConnected = 0x1110,
        ProvisioningAuthDataTooLarge = 0x1111,
        ProvisioningAlreadyInitialized = 0x1112,
        ProvisioningNotInitialized = 0x1113,
        ProvisioningConnecting = 0x1114,
        ProvisioningAlreadyConnected = 0x1115,
        ProvisioningNotConnected = 0x1116,
        ProvisioningIoError = 0x1117,
        ProvisioningInvalidTimeout = 0x1118,
        ProvisioningTS3ServerNotFound = 0x1119,
        ProvisioningNoPermission = 0x111A,

        //==== Rare errors ====
        //client
        ClientInvalidPassword = 0x0208,
        ClientTooManyClonesConnected = 0x0209,
        ClientIsOnline = 0x020b,

        //channel
        ChannelIsPrivateChannel = 0x030e,
        //note 0x030f is defined in common section above

        //database
        Database = 0x0500,
        DatabaseEmptyResult = 0x0501,
        DatabaseDuplicateEntry = 0x0502,
        DatabaseNoModifications = 0x0503,
        DatabaseConstraint = 0x0504,
        DatabaseReinvoke = 0x0505,

        //permissions
        PermissionInvalidGroupID = 0x0a00,
        PermissionDuplicateEntry = 0x0a01,
        PermissionInvalidPermID = 0x0a02,
        PermissionEmptyResult = 0x0a03,
        PermissionDefaultGroupForbidden = 0x0a04,
        PermissionInvalidSize = 0x0a05,
        PermissionInvalidValue = 0x0a06,
        PermissionsGroupNotEmpty = 0x0a07,
        PermissionsInsufficientGroupPower = 0x0a09,
        PermissionsInsufficientPermissionPower = 0x0a0a,
        PermissionTemplateGroupIsUsed = 0x0a0b,
        //note 0x0a0c is defined in common section above

        //server
        ServerDeploymentActive = 0x0405,
        ServerUnableToStopOwnServer = 0x0406,
        ServerWrongMachineid = 0x0408,
        ServerModalQuit = 0x040c,

        //messages
        MessageInvalidID = 0x0c00,

        //ban
        BanInvalidID = 0x0d00,
        ConnectFailedBanned = 0x0d01,
        RenameFailedBanned = 0x0d02,
        BanFlooding = 0x0d03,

        //tts
        TtsUnableToInitialize = 0x0e00,

        //privilege key
        PrivilegeKeyInvalid = 0x0f00,

        //voip
        VoipPjsua = 0x1000,
        VoipAlreadyInitialized = 0x1001,
        VoipTooManyAccounts = 0x1002,
        VoipInvalidAccount = 0x1003,
        VoipInternalError = 0x1004,
        VoipInvalidConnectionId = 0x1005,
        VoipCannotAnswerInitiatedCall = 0x1006,
        VoipNotInitialized = 0x1007,
    }
}
